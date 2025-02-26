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

void CalcMinkowskiPoints(const std::vector<Vector2>& meshA, const std::vector<Vector2>& meshB, std::vector<Vector2>* minkowskiPoints);

std::vector<Vector2> CnvexHull(std::vector<Vector2> vertexs);

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
	convex0.push_back({ 10.1f, 10.1f });
	convex0.push_back({ 50.0f, 00.0f });
	convex0.push_back({ 70.0f, 30.0f });
	convex0.push_back({ 40.0f, 60.0f });
	convex0.push_back({ 00.0f, 40.0f });

	std::vector<Vector2> convex1;
	convex1.push_back({ 20.0f, 20.0f });
	convex1.push_back({ 60.0f, 10.0f });
	convex1.push_back({ 90.0f, 40.0f });
	convex1.push_back({ 80.0f, 70.0f });
	convex1.push_back({ 50.0f, 90.0f });
	convex1.push_back({ 20.0f, 80.0f });
	convex1.push_back({ 00.0f, 50.0f });

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
		CalcMinkowskiPoints(convex0, convex1, &minkowskiPoints);

		minkowskiPoints = cnvexHull(minkowskiPoints);

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

		Novice::DrawLine((int)origin.x, (int)origin.y, (int)anyLine.x, (int)anyLine.y,BLACK);

		//DrawShape(box, RED);
		DrawShape(convex0, BLUE);
		DrawShape(convex1, GREEN);
		DrawShape(minkowskiPoints, RED);

		DrawTop(convex0, supportA);
		DrawTop(convex1, supportB);
		DrawTop(minkowskiPoints, Subtract(GetSupport(convex0, anyDirction), GetSupport(convex1, Multiply(-1.0f, anyDirction))));

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
