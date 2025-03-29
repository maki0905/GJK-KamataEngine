#include <Novice.h>

#include <algorithm>
#include <vector>

#include "Math.h"

const char kWindowTitle[] = "LE3B_15_マキ_ユキノリ";

Vector2 origin = { 360, 360 };

void DrawShape(std::vector<Vector2> obj, int color);
void DrawTop(std::vector<Vector2> obj, const Vector2& support);

void MoveConvex(std::vector<Vector2>* obj, const Vector2& value);

Vector2 GetSupport(const std::vector<Vector2>& polygon, const Vector2& dirction);

Vector2 MinkowskiSupport(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB, const Vector2& dirction);

void CalcMinkowskiPoints(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB, std::vector<Vector2>* minkowskiPoints);

std::vector<Vector2> CnvexHull(std::vector<Vector2> vertexs);

bool GJK(const std::vector<Vector2>& shapeA, const std::vector<Vector2>& shapeB, Vector2& outNormal, float& outDepth);

bool GJK(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB);

bool UpdateSimplex2(Vector2& a, Vector2& b, Vector2& searchDir);

bool UpdateSimplex3(Vector2& a, Vector2& b, Vector2& c);

bool ContainsOrigin(std::vector<Vector2>& simplex, Vector2& direction);

void EPA(std::vector<Vector2>& simplex, const std::vector<Vector2>& shapeA, const std::vector<Vector2>& shapeB, Vector2& outNormal, float& outDepth);

Vector2 Support(const std::vector<Vector2>& shapeA, const std::vector<Vector2>& shapeB, const Vector2& d) {
	auto maxPoint = [](const std::vector<Vector2>& shape, const Vector2& dir) {
		float maxDot = -INFINITY;
		Vector2 best;
		for (const auto& p : shape) {
			float dotVal = p.dot(dir);
			if (dotVal > maxDot) {
				maxDot = dotVal;
				best = p;
			}
		}
		return best;
		};

	Vector2 pointA = maxPoint(shapeA, d);
	Vector2 pointB = maxPoint(shapeB, -d);
	return pointA - pointB; // Minkowski Difference の点
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 720, 720);

	// 図形
	std::vector<Vector2> box;
	box.push_back({ 50.0f, 50.0f });
	box.push_back({ 100.0f, 50.0f });
	box.push_back({ 100.0f, 100.0f });
	box.push_back({ 50.0f, 100.0f });

	std::vector<Vector2> convex0;
	/*convex0.push_back({ -50.0f, -50.0f });
	convex0.push_back({ 50.0f, -50.0f });
	convex0.push_back({ -50.0f, -50.0f });
	convex0.push_back({ 50.0f, -50.0f });*/

	convex0.push_back({ 50.0f, 50.0f });
	convex0.push_back({ -50.0f, 50.0f });
	convex0.push_back({ -50.0f, -50.0f });
	convex0.push_back({ 50.0f, -50.0f });

	/*convex0.push_back({ 10.1f, 10.1f });
	convex0.push_back({ 50.0f, 00.0f });
	convex0.push_back({ 70.0f, 30.0f });
	convex0.push_back({ 40.0f, 60.0f });
	convex0.push_back({ 00.0f, 40.0f });*/


	std::vector<Vector2> convex1;
	/*convex1.push_back({ 0.0f, 0.0f });
	convex1.push_back({ 100.0f, 0.0f });
	convex1.push_back({ 100.0f, 100.0f });
	convex1.push_back({ 0.0f, 100.0f });*/

	convex1.push_back({ 0.0f, 100.0f });
	convex1.push_back({ 95.1f, 30.9f });
	convex1.push_back({ 58.8f , -80.9f });
	convex1.push_back({ -58.8f, -80.9f, });
	convex1.push_back({ -95.1f, 30.9f });

	/*convex1.push_back({ 0.0f, -100.0f });
	convex1.push_back({ 95.1f, -30.9f });
	convex1.push_back({ 58.8f , 80.9f });
	convex1.push_back({ -58.8f, 80.9f, });
	convex1.push_back({ -95.1f, -30.9f });*/
	/*convex1.push_back({ 20.0f, 20.0f });
	convex1.push_back({ 60.0f, 10.0f });
	convex1.push_back({ 90.0f, 40.0f });
	convex1.push_back({ 80.0f, 70.0f });
	convex1.push_back({ 50.0f, 90.0f });
	convex1.push_back({ 20.0f, 80.0f });
	convex1.push_back({ 00.0f, 50.0f });*/

	std::vector<Vector2> minkowskiPoints;

	Vector2 supportA;
	Vector2 supportB;

	Vector2 mousePosition;
	int mouseX = 0;
	int mouseY = 0;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Novice::GetMousePosition(&mouseX, &mouseY);
		mousePosition = { (float)mouseX, (float)mouseY };

		Vector2 anyDirction = Normalize(Subtract(mousePosition, origin));
		Vector2 anyLine = Add(origin, Multiply(100.0f, anyDirction));

		minkowskiPoints.clear();
		CalcMinkowskiPoints(convex1, convex0, &minkowskiPoints);

		minkowskiPoints = CnvexHull(minkowskiPoints);

		if (Novice::CheckHitKey(DIK_W)) {
			MoveConvex(&convex0, { 0.0f, -5.0f });
		}
		if (Novice::CheckHitKey(DIK_A)) {
			MoveConvex(&convex0, { -5.0f, 0.0f });
		}
		if (Novice::CheckHitKey(DIK_S)) {
			MoveConvex(&convex0, { 0.0f, 5.0f });
		}
		if (Novice::CheckHitKey(DIK_D)) {
			MoveConvex(&convex0, { 5.0f, 0.0f });
		}

		if (Novice::CheckHitKey(DIK_UP)) {
			MoveConvex(&convex1, { 0.0f, -5.0f });
		}
		if (Novice::CheckHitKey(DIK_LEFT)) {
			MoveConvex(&convex1, { -5.0f, 0.0f });
		}
		if (Novice::CheckHitKey(DIK_DOWN)) {
			MoveConvex(&convex1, { 0.0f, 5.0f });
		}
		if (Novice::CheckHitKey(DIK_RIGHT)) {
			MoveConvex(&convex1, { 5.0f, 0.0f });
		}

		supportA = GetSupport(convex0, anyDirction);
		supportB = GetSupport(convex1, anyDirction);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// X軸
		Novice::DrawLine(0, (int)origin.y, int(origin.x + origin.x), (int)origin.y, WHITE);
		// Y軸
		Novice::DrawLine((int)origin.x, 0, (int)origin.x, int(origin.y + origin.y), WHITE);

		//Novice::DrawLine((int)origin.x, (int)origin.y, (int)anyLine.x, (int)anyLine.y,BLACK);

		//DrawShape(box, RED);
		DrawShape(convex0, BLUE);
		DrawShape(convex1, GREEN);
		DrawShape(minkowskiPoints, RED);

		DrawTop(convex0, supportA);
		DrawTop(convex1, supportB);
		DrawTop(minkowskiPoints, Subtract(GetSupport(convex0, anyDirction), GetSupport(convex1, Multiply(-1.0f, anyDirction))));

		float depth;
		Vector2 normal;
		if (GJK(convex1, convex0, normal, depth)) {
			
			Novice::DrawEllipse((int)origin.x, (int)origin.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
		}

		//Novice::DrawEllipse(390, 290, 10, 10, 0.0f, BLACK, kFillModeSolid);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void DrawShape(std::vector<Vector2> obj, int color)
{
	Vector2 firstPoint = obj.front();

	for (auto it = obj.begin(); it != obj.end(); ++it) {
		Vector2 start = *it;
		Vector2 end;
		if (it == obj.end() - 1) {
			end = firstPoint;
		}
		else {
			end = *(std::next(it));
		}
		start = Add(origin, start);
		end = Add(origin, end);
		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, color);
	}

}

void DrawTop(std::vector<Vector2> obj, const Vector2& support)
{

	for (auto& vertex : obj) {
		if (vertex.x == support.x && vertex.y == support.y) {

			vertex = Add(origin, vertex);
			Novice::DrawEllipse((int)vertex.x, (int)vertex.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
		}
		else {
			vertex = Add(origin, vertex);
			Novice::DrawEllipse((int)vertex.x, (int)vertex.y, 1, 1, 0.0f, BLACK, kFillModeSolid);
		}
	}
}

void MoveConvex(std::vector<Vector2>* obj, const Vector2& value)
{
	for (auto& vertex : *obj) {
		vertex = Add(vertex, value);
	}
}

Vector2 GetSupport(const std::vector<Vector2>& polygon, const Vector2& dirction)
{
	Vector2 result = { 0.0f, 0.0f };
	float maxDot = -INFINITY;

	for (const Vector2& vertex : polygon) {
		float dotProduct = Dot(vertex, dirction);
		if (dotProduct > maxDot) {
			maxDot = dotProduct;
			result = vertex;
		}
	}

	return result;
}

Vector2 MinkowskiSupport(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB, const Vector2& dirction)
{
	return Subtract(GetSupport(meshA, dirction), GetSupport(meshB, Multiply(-1.0f, dirction)));
}

void CalcMinkowskiPoints(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB, std::vector<Vector2>* minkowskiPoints)
{
	for (auto& vertexA : meshA) {
		for (auto& vertexB : meshB) {
			minkowskiPoints->push_back(Subtract(vertexA, vertexB));
		}
	}
}

std::vector<Vector2> CnvexHull(std::vector<Vector2> vertexs)
{
	std::sort(vertexs.begin(), vertexs.end(), [](const Vector2& a, const Vector2& b) {
		if (a.x == b.x)
			return a.y < b.y;
		return a.x < b.x;
		});

	std::vector<Vector2> hull;

	for (const auto& p : vertexs) {
		while (hull.size() >= 2 && Cross(Subtract(hull[hull.size() - 1], hull[hull.size() - 2]), Subtract(p, hull[hull.size() - 2])) <= 0) {
			hull.pop_back();
		}
		hull.push_back(p);
	}

	size_t lower_size = hull.size();
	for (int i = (int)vertexs.size() - 2; i >= 0; --i) {
		while (hull.size() > lower_size && Cross(Subtract(hull[hull.size() - 1], hull[hull.size() - 2]), Subtract(vertexs[i], hull[hull.size() - 2])) <= 0) {
			hull.pop_back();
		}
		hull.push_back(vertexs[i]);
	}

	hull.pop_back();
	return hull;

}

bool GJK(const std::vector<Vector2>& shapeA, const std::vector<Vector2>& shapeB, Vector2& outNormal, float& outDepth)
{
	std::vector<Vector2> simplex;
	Vector2 direction = Vector2(1, 0);

	simplex.push_back(Support(shapeA, shapeB, direction));
	direction = -simplex[0];

	while (true) {
		Vector2 newPoint = Support(shapeA, shapeB, direction);

		if (newPoint.dot(direction) < 0) {
			return false; // 衝突なし
		}

		simplex.push_back(newPoint);

		if (ContainsOrigin(simplex, direction)) {
			Vector2 p0, p1, p2;
			p0 = Add(simplex[0], origin);
			p1 = Add(simplex[1], origin);
			p2 = Add(simplex[2], origin);

			Novice::DrawEllipse((int)p0.x, (int)p0.y, 3, 3, 0.0f, RED, kFillModeSolid);
			Novice::DrawEllipse((int)p1.x, (int)p1.y, 3, 3, 0.0f, BLUE, kFillModeSolid);
			Novice::DrawEllipse((int)p2.x, (int)p2.y, 3, 3, 0.0f, GREEN, kFillModeSolid);
			Novice::DrawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, BLACK);
			Novice::DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, BLACK);
			Novice::DrawLine((int)p2.x, (int)p2.y, (int)p0.x, (int)p0.y, BLACK);
			outDepth = 0.0f;
			outNormal = { 0.0f, 0.0f};
			break; // 衝突が確定したのでEPAへ
		}
	}

	//EPA(simplex, shapeA, shapeB, outNormal, outDepth);
	return true;
}

bool GJK(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB, Vector2& penetrationVector)
{
	Vector2 direction(1, 0); // 初期の探索方向（X軸方向）

	std::vector<Vector2> simplex;

	// 初期サポート点を計算
	Vector2 a = MinkowskiSupport(meshA, meshB, direction);
	simplex.push_back(a);

	direction = -a; // 原点に向かう方向へ更新

	// GJKループ
	for (int i = 0; i < 64; i++) {
		Vector2 newPoint = MinkowskiSupport(meshA, meshB, direction);
		simplex.push_back(newPoint);

		// 新しい点が原点方向に進んでいなければ、衝突なし
		if (Dot(newPoint, direction) < 0) {
			return false;
		}

		// Simplex 処理（原点を含むかチェックし、Simplex を更新）
		if (ContainsOrigin(simplex, direction)) {
			penetrationVector = {0.0f, 0.0f}/*EPA(meshA, meshB, simplex)*/;

			Edge closestEdge;
			Vector2 f;
			closestEdge.distance = FLT_MAX;

			for (size_t index = 0; index < simplex.size(); index++) {
				Vector2 p0 = simplex[index];
				int num = int((index + 1) % simplex.size());
				Vector2 p1 = simplex[num];

				Vector2 p0p1 = p1 - p0;
				Vector2 d = -p0;
				float t = Dot(d, p0p1.normalize()) / p0p1.length();
				f = p0 * (1 - t) + p1 * t;

				Vector2 normal = -f.normalize();

				float distance = f.length();

				if (distance < closestEdge.distance) {
					closestEdge = { p0, p1, distance, normal };
				}
			}

			Vector2 a0, b0, c0;
			Vector2 v;
			a0 = closestEdge.a + origin;
			b0 = closestEdge.b + origin;
			c0 = f + origin;
			//v = origin + f + closestEdge.normal * 100.0f;
			Novice::DrawLine((int)a0.x, (int)a0.y, (int)b0.x, (int)b0.y, WHITE);
			Novice::DrawEllipse((int)c0.x, (int)c0.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
			Novice::DrawLine((int)c0.x, (int)c0.y, (int)origin.x, (int)origin.y, 0x00FFFFFF);

			Vector2 p0, p1, p2;
			p0 = Add(simplex[0], origin);
			p1 = Add(simplex[1], origin);
			p2 = Add(simplex[2], origin);

			Novice::DrawEllipse((int)p0.x, (int)p0.y, 3, 3, 0.0f, RED, kFillModeSolid);
			Novice::DrawEllipse((int)p1.x, (int)p1.y, 3, 3, 0.0f, BLUE, kFillModeSolid);
			Novice::DrawEllipse((int)p2.x, (int)p2.y, 3, 3, 0.0f, GREEN, kFillModeSolid);
			Novice::DrawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, BLACK);
			Novice::DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, BLACK);
			Novice::DrawLine((int)p2.x, (int)p2.y, (int)p0.x, (int)p0.y, BLACK);

			v = Add(origin, penetrationVector.normalize() * 10.0f);
			Novice::DrawLine((int)origin.x, (int)origin.y, (int)v.x, (int)v.y, WHITE);

			return true; // 衝突あり
		}
	}

	return false;
}

bool GJK(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB) {
	Vector2 direction(1, 0); // 初期の探索方向（X軸方向）

	std::vector<Vector2> simplex;

	// 初期サポート点を計算
	Vector2 a = MinkowskiSupport(meshA, meshB, direction);
	simplex.push_back(a);

	direction = -a; // 原点に向かう方向へ更新

	// GJKループ
	for (int i = 0; i < 64; i++) {
		Vector2 newPoint = MinkowskiSupport(meshA, meshB, direction);
		simplex.push_back(newPoint);

		// 新しい点が原点方向に進んでいなければ、衝突なし
		if (Dot(newPoint, direction) < 0) {
			return false;
		}

		// Simplex 処理（原点を含むかチェックし、Simplex を更新）
		if (ContainsOrigin(simplex, direction)) {
			Vector2 p0, p1, p2;
			p0 = Add(simplex[0], origin);
			p1 = Add(simplex[1], origin);
			p2 = Add(simplex[2], origin);

			Novice::DrawEllipse((int)p0.x, (int)p0.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
			Novice::DrawEllipse((int)p1.x, (int)p1.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
			Novice::DrawEllipse((int)p2.x, (int)p2.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
			Novice::DrawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, BLACK);
			Novice::DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, BLACK);
			Novice::DrawLine((int)p2.x, (int)p2.y, (int)p0.x, (int)p0.y, BLACK);
			return true; // 衝突あり
		}
	}

	return false;
}

//direction = Normalize(Subtract(b, a));
//direction = { direction.y, direction.x };
//c = MinkowskiSupport(meshA, meshB, direction);
//simplex.push_back(c);
//
//Vector2 BC = Subtract(c, b);
//Vector2 B = Multiply(-1.0f, b);
//if (Dot({ -BC.y, BC.x }, B) > 0.0f) {
//	a = b;
//	b = c;
//	continue;
//}
//
//Vector2 AC = (Subtract(c, a));
//Vector2 A = Multiply(-1.0f, a);
//if (Dot({ -AC.y, AC.x }, A) > 0.0f) {
//	b = c;
//	continue;
//}
//
//
//a = Add(a, origin);
//b = Add(b, origin);
//c = Add(c, origin);
//
//Novice::DrawEllipse((int)a.x, (int)a.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
//Novice::DrawEllipse((int)b.x, (int)b.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
//Novice::DrawEllipse((int)c.x, (int)c.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
//return true;

bool UpdateSimplex2(Vector2& a, Vector2& b, Vector2& searchDir)
{
	Vector2 AO = Multiply(-1.0f, a);  // 原点方向
	Vector2 AB = Subtract(b, a);

	if (Dot(AB, AO) > 0.0f) {
		searchDir = Perpendicular(AB);
		return false;
	}
	searchDir = AO;
	b = a;

	a = Add(origin, a);
	b = Add(origin, b);

	Novice::DrawEllipse((int)a.x, (int)a.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
	Novice::DrawEllipse((int)b.x, (int)b.y, 3, 3, 0.0f, BLACK, kFillModeSolid);
	Novice::DrawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, BLACK);

	return true;
}

bool UpdateSimplex3(Vector2& a, Vector2& b, Vector2& c) {
	Vector2 ab = b - a;
	Vector2 ac = c - a;
	Vector2 ao = -a;

	Vector2 abPerp = Perpendicular(ab) * (Dot(Perpendicular(ab), ao) > 0 ? 1.0f : -1.0f);
	Vector2 acPerp = Perpendicular(ac) * (Dot(Perpendicular(ac), ao) > 0 ? 1.0f : -1.0f);

	if (Dot(acPerp, ao) > 0) {
		b = c;
		return false;
	}
	if (Dot(abPerp, ao) > 0) {
		c = b;
		return false;
	}

	return true; // 原点を囲んでいる
}

bool ContainsOrigin(std::vector<Vector2>& simplex, Vector2& direction)
{
	Vector2 A = simplex.back();  // 最新の点
	Vector2 AO = -A;             // 原点へのベクトル

	if (simplex.size() == 2) {
		// 線分の場合
		Vector2 B = simplex[0];
		Vector2 AB = B - A;

		// 原点が線分のどちら側にあるか
		Vector2 AB_perp = Perpendicular(AB);
		if (Dot(AB_perp, AO) < 0) {
			AB_perp = -AB_perp;
		}
		direction = AB_perp;
	}
	else if (simplex.size() == 3) {
		// 三角形の場合
		Vector2 B = simplex[1];
		Vector2 C = simplex[0];

		Vector2 AB = B - A;
		Vector2 AC = C - A;

		// 三角形のエッジの法線を求める
		Vector2 AB_perp = Perpendicular(AB);
		if (Dot(AB_perp, AC) > 0) {
			AB_perp = -AB_perp;
		}

		Vector2 AC_perp = Perpendicular(AC);
		if (Dot(AC_perp, AB) > 0) {
			AC_perp = -AC_perp;
		}

		// 原点がどこにあるか判定
		if (Dot(AB_perp, AO) > 0) {
			simplex.erase(simplex.begin()); // C を削除
			direction = AB_perp;
		}
		else if (Dot(AC_perp, AO) > 0) {
			simplex.erase(simplex.begin() + 1); // B を削除
			direction = AC_perp;
		}
		else {
			return true; // 原点は三角形の内部
		}
	}

	return false;
}

void EPA(std::vector<Vector2>& simplex, const std::vector<Vector2>& shapeA, const std::vector<Vector2>& shapeB, Vector2& outNormal, float& outDepth)
{
	struct Face {
		Vector2 a, b, c;
		Vector2 normal;
		float distance;
	};

	std::vector<Face> polytope;

	// 初期ポリトープの構築
	polytope.push_back({ simplex[0], simplex[1], simplex[2], Vector2(), 0.0f });

	while (true) {
		// 最も原点に近い面を見つける
		int closestFaceIdx = -1;
		float minDistance = FLT_MAX;

		for (size_t i = 0; i < polytope.size(); ++i) {
			Face& f = polytope[i];
			Vector2 edge1 = f.b - f.a;
			Vector2 edge2 = f.c - f.a;
			f.normal = Vector2(-edge1.y, edge1.x).normalize();
			f.distance = f.normal.dot(f.a);

			if (f.distance < minDistance) {
				minDistance = f.distance;
				closestFaceIdx = int(i);
			}
		}

		// 新しいサポート点を取得
		Face& closestFace = polytope[closestFaceIdx];
		Vector2 newPoint = Support(shapeA, shapeB, closestFace.normal);
		float newDistance = closestFace.normal.dot(newPoint);

		// 収束判定: 新しい点がほぼ同じ位置なら終了
		if (std::abs(newDistance - minDistance) < 0.001f) {
			outNormal = closestFace.normal;
			outDepth = newDistance;
			return;
		}

		// 新しい点を使ってポリトープを拡張
		std::vector<Edge> edges;
		for (size_t i = 0; i < polytope.size(); ++i) {
			Face& f = polytope[i];
			if (f.normal.dot(newPoint - f.a) > 0) {
				edges.push_back({ f.a, f.b });
				edges.push_back({ f.b, f.c });
				edges.push_back({ f.c, f.a });
				polytope.erase(polytope.begin() + i);
				--i;
			}
		}

		// 新しい面を追加
		for (size_t i = 0; i < edges.size(); ++i) {
			polytope.push_back({ edges[i].a, edges[i].b, newPoint, Vector2(), 0.0f });
		}
	}
}



