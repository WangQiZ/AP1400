#include "hw1.h"

Matrix algebra::zeros(size_t n, size_t m) {
    Matrix matrix(n);
    for(auto &row : matrix) {
        row = std::vector<double>(m,0);
    }
    return matrix;
}

Matrix algebra::ones(size_t n, size_t m) {
    Matrix matrix(n);
    for(auto &row : matrix) {
        row = std::vector<double>(m,1);
    }
    return matrix;
}

Matrix algebra::random(size_t n, size_t m, double min, double max) {
    if(min > max) {
        throw std::logic_error("min could not be greater than max.");
    }

    Matrix matrix(n);
    for(auto &row : matrix) {
        row = std::vector<double>(m,0);
        for(auto &col : row) {
            std::random_device seed;//硬件生成随机数种子
            std::ranlux48 engine(seed());//利用种子生成随机数引擎
            std::uniform_int_distribution<> distrib(min, max-1);//设置随机数范围，并为均匀分布
            int random = distrib(engine);//随机数
            col = random;
        }
    }
    return matrix;
}

void algebra::show(const Matrix& matrix) {
    for(auto row : matrix) {
        for(auto col : row) {
            std::cout << std::setw(7) << std::setprecision(3) << std::fixed << col;
        }
        std::cout << std::endl;
    }
}

Matrix copy(const Matrix& matrix) {
    int num_row = matrix.size();
    int num_col = matrix[0].size();
    Matrix out(num_row);
    for(auto &row : out) {
        row = std::vector<double>(num_col, 0);
    }

    for(int i = 0; i < num_row; i++) {
        for(int j = 0; j < num_col; j++) {
            out[i][j] = matrix[i][j];
        }
    }
    return out;
}

Matrix algebra::multiply(const Matrix& matrix, double c) {
    Matrix out = copy(matrix);
    for(auto &row : out) {
        for(auto &col : row) {
            col = col * c;
        }
    }
    return out;
}

double dotmultiply(const Matrix& matrix1, const Matrix& matrix2, int row, int col) {
    double out = 0;
    for(int i = 0; i < matrix2.size(); i++) {
        out += matrix1[row][i] * matrix2[i][col];
    }
    return out;
}

Matrix algebra::multiply(const Matrix& matrix1, const Matrix& matrix2) {
    if(matrix1.empty() || matrix2.empty()) {
        return Matrix{};
    }
    int row = matrix1.size();
    int col = matrix2[0].size();
    int m1 = matrix1[0].size();
    int m2 = matrix2.size();
    if(m1 != m2) {
        throw std::logic_error("could not multiply.");
    }
    Matrix out = zeros(row, col);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            out[i][j] = dotmultiply(matrix1, matrix2, i, j);
        }
    }
    return out;
}

Matrix algebra::sum(const Matrix& matrix, double c) {
    if(matrix.empty()) {
        return Matrix{};
    }
    int row = matrix.size();
    int col = matrix[0].size();
    Matrix out = zeros(row, col);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            out[i][j] = matrix[i][j] + c;
        }
    }
    return out;
}
Matrix algebra::sum(const Matrix& matrix1, const Matrix& matrix2) {
    if(matrix1.empty() && matrix2.empty()) {
        return Matrix{};
    }
    if(matrix1.empty() || matrix2.empty()) {
        throw std::logic_error("error");
    }
    int matrix1_row = matrix1.size();
    int matrix1_col = matrix1[0].size();
    int matrix2_row = matrix2.size();
    int matrix2_col = matrix2[0].size();
    if(matrix1_row != matrix2_row || matrix1_col != matrix2_col) {
        throw std::logic_error("error.");
    }
    Matrix out = zeros(matrix1_row, matrix1_col);
    for(int i = 0; i < matrix1_row; i++) {
        for(int j = 0; j < matrix1_col; j++) {
            out[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return out;
}

Matrix algebra::transpose(const Matrix& matrix) {
    if(matrix.empty()) {
        return Matrix{};
    }
    int row = matrix.size();
    int col = matrix[0].size();
    Matrix out = zeros(col, row);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            out[j][i] = matrix[i][j];
        }
    }
    return out;
}

//remove row:n col:m
Matrix algebra::minor(const Matrix& matrix, size_t n, size_t m) {
    int row = matrix.size();
    int col = matrix[0].size();
    Matrix out = zeros(row - 1, col - 1);
    int out_i = 0, out_j = 0;
    for(int i = 0; i < row; i++) {
        if(i == n)
            continue;
        for(int j = 0; j < col; j++) {
            if(j == m) {
                continue;
            } else {
                out[out_i][out_j++] = matrix[i][j];
            }
        }
        out_i++;
        out_j = 0;
    }
    return out;
}

double algebra::determinant(const Matrix& matrix) {
    if(matrix.empty()) {
        return 1;
    }
    int row = matrix.size();
    int col = matrix[0].size();
    if(row != col) {
        throw std::logic_error("error");
    }

    if(col == 1) {
        return matrix[0][0];
    }
    double determinant = 0;
    int flag = 1;
    for(int i = 0 ; i < col; i++) {
        determinant += flag * matrix[0][i] * algebra::determinant(minor(matrix, 0, i));
        flag = -flag;
    }
    return determinant;
}