#include <iostream> 
#include <thread>
#include <chrono>
#include <random>
#include <cstdint>
#include <cassert>
#include <typeinfo>

void sleep(const std::int64_t& time) {
  std::this_thread::sleep_for(std::chrono::milliseconds{ time });
}

class Cell {
  friend class Matrix;

public:
  enum class CellType {
    DEAD = 0,
    ALIVE = 1,
  };

  CellType& Type() {
    return m_CellType;
  }
  const CellType Type() const {
    return m_CellType;
  }

  operator std::int32_t() {
    return static_cast<std::int32_t>(m_CellType);
  }

  friend std::ostream& operator<<(std::ostream& os, const Cell& cell) {
    switch (cell.m_CellType) {
    case CellType::ALIVE:
      os << Cell::CELL_ALIVE;
      break;
    case CellType::DEAD:
      os << Cell::CELL_DEAD;
      break;
    default:
      assert(false && "[ERROR] Invalid cell type.");
    }
    return os;
  }

private:
  static constexpr std::uint8_t CELL_ALIVE = 254;
  static constexpr std::uint8_t CELL_DEAD = 32;

  CellType m_CellType;
};

class row_t {
public:
  row_t(Cell* row, std::int32_t col) :
    m_Col{ col },
    m_Data{ row } {
  }

  Cell& operator[](std::int32_t offset) const {
    assert(offset >= 0 && offset < m_Col);
    return *(m_Data + offset);
  }

private:
  std::int32_t m_Col;
  Cell* m_Data;
};

class Matrix {
public:
  Matrix(std::int32_t row) :
    Matrix{ row, row } {
  }
  Matrix(std::int32_t row, std::int32_t col) :
    m_Row{ row }, m_Col{ col },
    m_Data{ new Cell[row * col]{} } {
    assert(row >= 3 && col >= 3);
  }

  ~Matrix() {
    delete[] m_Data;
  }

  Cell& at(std::int32_t row, std::int32_t col) {
    assert(row >= 0 && row < m_Row);
    assert(col >= 0 && col < m_Col);
    return m_Data[row * m_Col + col];
  }

  row_t operator[](std::int32_t offset) const {
    assert(offset >= 0 && offset < m_Row);
    return row_t{ m_Data + (offset * m_Col), m_Col };
  }

  void random_fill(std::uint32_t seed) {
    std::default_random_engine rng{ seed };
    std::uniform_int_distribution<std::uint32_t> dist{ 1, 100 };

    for (std::int32_t i = 0; i < m_Row; i++) {
      for (std::int32_t j = 0; j < m_Col; j++) {
        if (dist(rng) <= SUCCESS_RATE) {
          m_Data[i * m_Col + j].Type() = Cell::CellType::ALIVE;
        }
      }
    }
  }

  void next_gen() {
    Cell* copy = new Cell[m_Row * m_Col];
    for (std::int32_t i = 0; i < m_Row; i++) {
      for (std::int32_t j = 0; j < m_Col; j++) {
        copy[i * m_Col + j] = m_Data[i * m_Col + j];
      }
    }

    auto copyAt = [&](std::int32_t i, std::int32_t j) {
      return copy[i * m_Col + j];
    };

    for (std::int32_t i = 0; i < m_Row; i++) {
      for (std::int32_t j = 0; j < m_Col; j++) {
        std::int32_t liveCells = 0;

        if (i == 0) {
          liveCells += copyAt((i + 1), (j + 0));

          if (j == 0) {
            liveCells += copyAt((i + 0), (j + 1)) + copyAt((i + 1), (j + 1));
          }
          else if (j == m_Col - 1) {
            liveCells += copyAt((i + 0), (j - 1)) + copyAt((i + 1), (j - 1));
          }
          else {
            liveCells += copyAt((i + 0), (j - 1)) + copyAt((i + 0), (j + 1)) + copyAt((i + 1), (j - 1)) + copyAt((i + 1), (j + 1));
          }
        }
        else if (i == m_Row - 1) {
          liveCells += copyAt((i - 1), (j + 0));

          if (j == 0) {
            liveCells += copyAt((i - 1), (j + 1)) + copyAt((i + 0), (j + 1));
          }
          else if (j == m_Col - 1) {
            liveCells += copyAt((i - 1), (j - 1)) + copyAt((i + 0), (j - 1));
          }
          else {
            liveCells += copyAt((i - 1), (j - 1)) + copyAt((i - 1), (j + 1)) + copyAt((i + 0), (j - 1)) + copyAt((i + 0), (j + 1));
          }
        }
        else {
          liveCells += copyAt((i - 1), (j + 0)) + copyAt((i + 1), (j + 0));

          if (j == 0) {
            liveCells += copyAt((i - 1), (j + 1)) + copyAt((i + 0), (j + 1)) + copyAt((i + 1), (j + 1));
          }
          else if (j == m_Col - 1) {
            liveCells += copyAt((i - 1), (j - 1)) + copyAt((i + 0), (j - 1)) + copyAt((i + 1), (j - 1));
          }
          else {
            liveCells += copyAt((i - 1), (j - 1)) + copyAt((i - 1), (j + 1)) + copyAt((i + 0), (j - 1)) + copyAt((i + 0), (j + 1)) + copyAt((i + 1), (j - 1)) + copyAt((i + 1), (j + 1));
          }
        }

        if (liveCells < 2) { m_Data[i * m_Col + j].Type() = Cell::CellType::DEAD; }
        else if (liveCells == 3) { m_Data[i * m_Col + j].Type() = Cell::CellType::ALIVE; }
        else if (liveCells > 3) { m_Data[i * m_Col + j].Type() = Cell::CellType::DEAD; }
      }
    }

    delete[] copy;
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (std::int32_t i = 0; i < matrix.m_Row; i++) {
      for (std::int32_t j = 0; j < matrix.m_Col; j++) {
        os << " " << matrix[i][j];
      }
      os << "\n";
    }
    return os;
  }

private:
  static constexpr std::int32_t SUCCESS_RATE = 40;

  std::int32_t m_Row, m_Col;
  Cell* m_Data;

};

int main() {
  Matrix m{ 20, 20 };
  m.random_fill(static_cast<std::uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));

  while (true) {
    std::cout << m;
    sleep(50);
    m.next_gen();
    std::system("CLS");
  }
}