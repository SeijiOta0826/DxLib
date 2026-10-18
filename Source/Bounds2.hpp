#pragma once

# include "Vector.hpp"
# include "GraphicsManager.hpp"
# include "Camera.hpp"

// AABB
class Bounds2 {
public:
	Vector2f p_min, p_max;

	// ctor
	Bounds2() {
		constexpr float min_num = (std::numeric_limits<float>::lowest)();
		constexpr float max_num = (std::numeric_limits<float>::max)();

		p_max = Vector2f(min_num, min_num);
		p_min = Vector2f(max_num, max_num);
	}
	Bounds2(const Vector2f& p) : p_max(p), p_min(p) {}
	Bounds2(const Vector2f& p1, const Vector2f& p2) {
		p_min = Vector2f(fmin(p1.x, p2.x), fmin(p1.y, p2.y));
		p_max = Vector2f(fmax(p1.x, p2.x), fmax(p1.y, p2.y));
	}

	// ボックス中心点
	Vector2f Centroid() const { return (0.5f * p_min + 0.5f * p_max); }

	// 中心点とサイズを設定することでボックスを生成（size : (w, h)）
	static Bounds2 FromCenterSize(const Vector2f& center, const Vector2f& size) {
		Vector2f halfSize = size * 0.5f;
		return Bounds2(center - halfSize, center + halfSize);
	}

	// 二つのボックスが重なっているかどうか
	bool Overlaps(const Bounds2& b1, const Bounds2& b2) {
		bool x = (b1.p_max.x >= b2.p_min.x) && (b1.p_min.x <= b2.p_max.x);
		bool y = (b1.p_max.y >= b2.p_min.y) && (b1.p_min.y <= b2.p_max.y);
		return (x && y);
	}

	bool Overlaps(const Bounds2& other) const {
		bool x = (p_max.x >= other.p_min.x) && (p_min.x <= other.p_max.x);
		bool y = (p_max.y >= other.p_min.y) && (p_min.y <= other.p_max.y);
		return (x && y);
	}

	// 点 p がボックス内にいるかどうか
	bool Inside(const Vector2f& p, const Bounds2& b) {
		return (
			((p.x >= b.p_min.x) && (p.x <= b.p_max.x)) &&
			((p.y >= b.p_min.y) && (p.y <= b.p_max.y))
			);
	}

	// テスト用：ボックスを描画する（座標変換済）
	void DrawAABB(const Camera& camera) {
		Vector2f sMin = camera.WorldToScreen(p_min);
		Vector2f sMax = camera.WorldToScreen(p_max);

		GraphicsManager::GetInstance().DrawLine(sMin.x, sMin.y, sMin.x, sMax.y, GetColor(255, 0, 0)); // Left
		GraphicsManager::GetInstance().DrawLine(sMin.x, sMin.y, sMax.x, sMin.y, GetColor(255, 0, 0)); // Top
		GraphicsManager::GetInstance().DrawLine(sMax.x, sMin.y, sMax.x, sMax.y, GetColor(255, 0, 0)); // Right
		GraphicsManager::GetInstance().DrawLine(sMin.x, sMax.y, sMax.x, sMax.y, GetColor(255, 0, 0)); // Bottom
	}
};