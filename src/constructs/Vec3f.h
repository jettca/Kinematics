#pragma once

#include <iostream>

namespace d3d {
	class Vec3f  {
	public:
		float x, y, z;

		Vec3f()
		{}

		Vec3f(float X, float Y, float Z)
			: x(X), y(Y), z(Z)
		{}

		bool operator==(const Vec3f &other) const {
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(const Vec3f &other) const {
			return x != other.x || y != other.y || z != other.z;
		}

		Vec3f operator*(float scale) const {
			return Vec3f(x * scale, y * scale, z * scale);
		}

		Vec3f operator/(float scale) const {
			float scaleInv = 1.0f / scale;
			return Vec3f(x * scaleInv, y * scaleInv, z * scaleInv);
		}

		Vec3f operator+(const Vec3f &other) const {
			return Vec3f(x + other.x, y + other.y, z + other.z);
		}

		Vec3f operator-(const Vec3f &other) const {
			return Vec3f(x - other.x, y - other.y, z - other.z);
		}

		Vec3f operator*(const Vec3f &other) const {
			return Vec3f(x * other.x, y * other.y, z * other.z);
		}

		Vec3f operator/(const Vec3f &other) const {
			return Vec3f(x / other.x, y / other.y, z / other.z);
		}

		Vec3f operator-() const {
			return Vec3f(-x, -y, -z);
		}

		const Vec3f &operator*=(float scale);
		const Vec3f &operator/=(float scale);
		const Vec3f &operator+=(const Vec3f &other);
		const Vec3f &operator-=(const Vec3f &other);
		const Vec3f &operator*=(const Vec3f &other);
		const Vec3f &operator/=(const Vec3f &other);

		float magnitude() const {
			return std::sqrtf(x * x + y * y + z * z);
		}

		float magnitudeSquared() const {
			return x * x + y * y + z * z;
		}

		void normalize();
		Vec3f normalized() const;

		float dot(const Vec3f &other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		Vec3f cross(const Vec3f &other) const;
		Vec3f project(const Vec3f &other) const; // Projection of this on to other

		Vec3f lerp(const Vec3f &first, const Vec3f &second, float interpolationCoefficient) {
			return first + (second - first) * interpolationCoefficient;
		}

		// Hash
		size_t operator()(const Vec3f &vec) const {
			return static_cast<size_t>(vec.x + vec.y * 1355.0f + vec.z * 1836025.0f);
		}
	};

	static Vec3f operator*(float scale, const Vec3f &v) {
		return v * scale;
	}

	std::ostream &operator<<(std::ostream &output, const Vec3f &v);
}