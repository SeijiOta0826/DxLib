#pragma once
# include <iostream>

// ２次元ベクトルのクラス
class Vector2f {
public:
	float x, y;

	// ctor
	Vector2f() : x(0.f), y(0.f) {}
	Vector2f(float init) : x(init), y(init) {}
	Vector2f(float xx, float yy) : x(xx), y(yy) {}

	// オペレーターオーバロード（ベクトルの足し算・引き算・掛け算・割り算）
	Vector2f operator + (const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }
	Vector2f operator += (const Vector2f& v) { return Vector2f(x += v.x, y += v.y); }

	Vector2f operator - (const Vector2f& v) const { return Vector2f(x - v.x, y - v.y); }
	Vector2f operator - () const { return Vector2f(-x, -y); }

	Vector2f operator * (const Vector2f& v) const { return Vector2f(x * v.x, y * v.y); }
	Vector2f operator * (const float& a) const { return Vector2f(a * x, a * y); }
	friend Vector2f operator * (const float& a, const Vector2f& v) { return Vector2f(a * v.x, a * v.y); }

	Vector2f operator / (const float& a) const { return Vector2f(x / a, y / a); }

	// 長さ
	float magnitude() { return std::sqrtf(x * x + y * y); }
	
	// 規格化
	Vector2f normalize() {
		float mag = std::sqrtf(x * x + y * y);
		float inv_mag = 1 / mag;

		return Vector2f(x * inv_mag, y * inv_mag);
	}

	// テスト用：ベクトル v を（v.x, v.y）の形で書き出す
	friend std::ostream& operator << (std::ostream& output, const Vector2f& v) {
		return output << "(" << v.x << "," << v.y << ")";
	}

} typedef vec2f;

// 内積
inline float DotProduct(const Vector2f& v1, const Vector2f& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

// 規格化
inline Vector2f Normalize(const Vector2f& v) {
	float mag_square = v.x * v.x + v.y * v.y;

	if (mag_square > 0) {
		float mag = std::sqrtf(mag_square);
		float inv_mag = 1.f / mag;
		return Vector2f(v.x * inv_mag, v.y * inv_mag);
	}

	return v;
}


// ３次元ベクトルのクラス
// 念のために置いといた、現時点必要ない？
class Vector3f {
public:
	float x, y, z;

	// ctor
	Vector3f() : x(0.f), y(0.f), z(0.f) {}
	Vector3f(float init) : x(init), y(init), z(init) {}
	Vector3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

	Vector3f operator + (const Vector3f& v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
	Vector3f operator += (const Vector3f& v) { return Vector3f(x += v.x, y += v.y, z += v.z); }

	Vector3f operator - (const Vector3f& v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }
	Vector3f operator - () const { return Vector3f(-x, -y, -z); }

	Vector3f operator * (const float& a) const { return Vector3f(a * x, a * y, a * z); }
	Vector3f operator * (const Vector3f& v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }
	friend Vector3f operator * (const float& a, const Vector3f& v) { return Vector3f(a * v.x, a * v.y, a * v.z); }

	Vector3f operator / (const float& a) const { return Vector3f(x / a, y / a, z / a); }

	float magnitude() { return std::sqrtf(x * x + y * y + z * z); }
	Vector3f normalize() {
		float mag = std::sqrtf(x * x + y * y + z * z);
		float inv_mag = 1 / mag;

		return Vector3f(x * inv_mag, y * inv_mag, z * inv_mag);
	}

	friend std::ostream& operator << (std::ostream& output, const Vector3f& v) {
		return output << "(" << v.x << "," << v.y << "," << v.z << ")";
	}

	static Vector3f Min(const Vector3f& v1, const Vector3f& v2) {
		return Vector3f((std::min)(v1.x, v2.x), (std::min)(v1.y, v2.y), (std::min)(v1.z, v2.z));
	}
	static Vector3f Max(const Vector3f& v1, const Vector3f& v2) {
		return Vector3f((std::max)(v1.x, v2.x), (std::max)(v1.y, v2.y), (std::max)(v1.z, v2.z));
	}

	double  operator[](int index) const { return (&x)[index]; }
	double& operator[](int index);

} typedef vec3f; 