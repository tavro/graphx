template <class T> 
struct Vector2D {
    T x = 0;
	T y = 0;

	Vector2D() : x(0), y(0) {}
	Vector2D(T _x, T _y) : x(_x), y(_y) {}
	Vector2D(const Vector2D& v) : x(v.x), y(v.y) {}

	Vector2D& operator=(const Vector2D& v) = default;
		
    T mag () const { return T(std::sqrt(x * x + y * y)); }
	T mag2() const { return x * x + y * y; }

	Vector2D  norm() const { 
        T r = 1 / mag(); 
        return Vector2D(x * r, y * r); 
    }

	Vector2D  perp () const { return Vector2D(-y, x); }
	Vector2D  floor() const { return Vector2D(std::floor(x), std::floor(y)); }
	Vector2D  ceil () const { return Vector2D(std::ceil(x), std::ceil(y)); }

	Vector2D  max(const Vector2D& v) const { return Vector2D(std::max(x, v.x), std::max(y, v.y)); }
	Vector2D  min(const Vector2D& v) const { return Vector2D(std::min(x, v.x), std::min(y, v.y)); }

	Vector2D  cart () { return { std::cos(y) * x, std::sin(y) * x }; }
	Vector2D  polar() { return { mag(), std::atan2(y, x) }; }

	Vector2D  clamp(const Vector2D& v1, const Vector2D& v2) const { return this->max(v1)->min(v2); }
	Vector2D  lerp (const Vector2D& v1, const double t) { return this->operator*(T(1.0 - t)) + (v1 * T(t)); }

	T dot  (const Vector2D& rhs) const { return this->x * rhs.x + this->y * rhs.y; }
	T cross(const Vector2D& rhs) const { return this->x * rhs.y - this->y * rhs.x; }

	Vector2D  operator +  (const Vector2D& rhs) const { return Vector2D(this->x + rhs.x, this->y + rhs.y); }
	Vector2D  operator -  (const Vector2D& rhs) const { return Vector2D(this->x - rhs.x, this->y - rhs.y); }

	Vector2D  operator *  (const T& rhs)           const { return Vector2D(this->x * rhs, this->y * rhs); }
	Vector2D  operator *  (const Vector2D& rhs)    const { return Vector2D(this->x * rhs.x, this->y * rhs.y); }
	Vector2D  operator /  (const T& rhs)           const { return Vector2D(this->x / rhs, this->y / rhs); }
	Vector2D  operator /  (const Vector2D& rhs)    const { return Vector2D(this->x / rhs.x, this->y / rhs.y); }

	Vector2D& operator += (const Vector2D& rhs) { 
        this->x += rhs.x; this->y += rhs.y; 
        return *this; 
    }

	Vector2D& operator -= (const Vector2D& rhs) { 
        this->x -= rhs.x; this->y -= rhs.y; 
        return *this; 
    }

	Vector2D& operator *= (const T& rhs) { 
        this->x *= rhs; this->y *= rhs; 
        return *this; 
    }

	Vector2D& operator /= (const T& rhs) { 
        this->x /= rhs; this->y /= rhs; 
        return *this; 
    }

	Vector2D& operator *= (const Vector2D& rhs) { 
        this->x *= rhs.x; this->y *= rhs.y; 
        return *this; 
    }

	Vector2D& operator /= (const Vector2D& rhs) { 
        this->x /= rhs.x; this->y /= rhs.y; 
        return *this; 
    }

	Vector2D  operator +  () const { return { +x, +y }; }
	Vector2D  operator -  () const { return { -x, -y }; }

	bool operator == (const Vector2D& rhs) const { return (this->x == rhs.x && this->y == rhs.y); }
	bool operator != (const Vector2D& rhs) const { return (this->x != rhs.x || this->y != rhs.y); }

	const std::string str() const { return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + ")"; }

	friend std::ostream& operator << (std::ostream& os, const Vector2D& rhs) { os << rhs.str(); return os; }

	operator Vector2D<int32_t>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
	operator Vector2D<float>  () const { return { static_cast<float>(this->x), static_cast<float>(this->y) }; }
	operator Vector2D<double> () const { return { static_cast<double>(this->x), static_cast<double>(this->y) }; }
};

template<class T> inline Vector2D<T> operator * (const float& lhs, const Vector2D<T>& rhs) { 
    return Vector2D<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y)); 
}

template<class T> inline Vector2D<T> operator * (const double& lhs, const Vector2D<T>& rhs) { 
    return Vector2D<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y)); 
}

template<class T> inline Vector2D<T> operator * (const int& lhs, const Vector2D<T>& rhs) { 
    return Vector2D<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y)); 
}

template<class T> inline Vector2D<T> operator / (const float& lhs, const Vector2D<T>& rhs) { 
    return Vector2D<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y)); 
}

template<class T> inline Vector2D<T> operator / (const double& lhs, const Vector2D<T>& rhs) { 
    return Vector2D<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y)); 
}

template<class T> inline Vector2D<T> operator / (const int& lhs, const Vector2D<T>& rhs) { 
    return Vector2D<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y)); 
}

template<class T, class U> inline bool operator < (const Vector2D<T>& lhs, const Vector2D<U>& rhs) { 
    return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x); 
}

template<class T, class U> inline bool operator > (const Vector2D<T>& lhs, const Vector2D<U>& rhs) { 
    return lhs.y > rhs.y || (lhs.y == rhs.y && lhs.x > rhs.x); 
}

typedef Vector2D<int32_t>  int_vector_2d;
typedef Vector2D<uint32_t> unsigned_vector_2d;
typedef Vector2D<float>    float_vector_2d;
typedef Vector2D<double>   double_vector_2d;