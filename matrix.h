#ifndef MATRIX_H_
#define MATRIX_H_

#include <memory>
template <class T>
class Matrix{
public:
	typedef T ElementType;
	//functions
	inline size_t width() const {return width_;}
	inline size_t height() const {return height_;}
	T** data() {return matrix;}
	//operators
	inline T& operator()(size_t h, size_t w) {return matrix[h][w];}
	inline Matrix& operator=(const Matrix &m);
	inline T* operator[](size_t h) {return matrix[h];}
	inline const T* operator[](size_t h) const {return matrix[h];}
	//constructors
	explicit Matrix(size_t size) {initMatrix(size, size);}
	Matrix(T** m, size_t size) {initMatrix(m, size, size);}
	Matrix(T** m, size_t height_, size_t width_) {initMatrix(m, height_, width_);}
	Matrix(const Matrix &m){initMatrix(m.matrix, m.height(), m.width());}
	//destructor
	~Matrix(){reset();}

protected:
	T** matrix;
	size_t width_;
	size_t height_;
	//private functions
	void reset();	//make private
	void initMatrix(T**, size_t, size_t);
	void initMatrix(size_t, size_t);
};

template<class T>
void Matrix<T>::initMatrix(T** m, size_t h, size_t w){
	width_  = w;
	height_ = h;
	matrix = new T*[height()];
	for (size_t i = 0; i < height(); i++){
		matrix[i] = new T[width()];
		for (size_t j = 0; j < width(); j++)
			matrix[i][j] = m[i][j];
	}
}

template<class T>
void Matrix<T>::initMatrix(size_t height_, size_t width_){
	this->height_ = height_;
	this->width_  = width_;
	matrix = new T*[height()];
	for (size_t i = 0; i < height(); i++)
		matrix[i] = new T[width()];
}


template<class T>
void Matrix<T>::reset(){
	if(matrix != nullptr){
		for (size_t i = 0; i < height(); i++)
			delete[] matrix[i];
		delete[] matrix;
		matrix = nullptr;
	}
	width_  = 0;
	height_ = 0;
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix &m){
	reset();
	initMatrix(m.matrix, m.height(), m.width());
	return *this;
}

#endif /* MATRIX_H_ */
