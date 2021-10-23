#include <algorithm>
#include "triangle.h"
#include "render.h"

namespace bl {

	Triangle::Triangle(Vertex** pts) {
		this->pts[0] = pts[0];
		this->pts[1] = pts[1];
		this->pts[2] = pts[2];
	}


	void Triangle::draw_f(const Vec3f& color_e) const {
		// conv from world-space to camera-space
		Vec3f triCam[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCam[a] = RenderBL::cam.getCameraCoord(pts[a]->pos);
		}

		// only draw tris facing camera
		Vec3f normalCam = crossProduct(triCam[1] - triCam[0], triCam[2] - triCam[0]);
		if (dotProduct(triCam[0], normalCam) < 0.0f) {
			if (clip_f(triCam, color_e)) raster_f(triCam, color_e);
		}
	}


	void Triangle::draw_v() const {
		// conv from world-space to camera-space
		Vec3f triCam[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCam[a] = RenderBL::cam.getCameraCoord(pts[a]->pos);
		}

		// only draw tris facing camera
		Vec3f normalCam = crossProduct(triCam[1] - triCam[0], triCam[2] - triCam[0]);
		if (dotProduct(triCam[0], normalCam) < 0.0f) {
			Vec3f tri_c[3] = { pts[0]->color, pts[1]->color, pts[2]->color }; // vertex colors
			if (clip_v(triCam, tri_c)) raster_v(triCam, tri_c);
		}
	}


	Vec3f Triangle::getNormal() const {
		Vec3f vec1 = pts[1]->pos - pts[0]->pos;
		Vec3f vec2 = pts[2]->pos - pts[0]->pos;
		return crossProduct(vec1, vec2).getNormalized();
	}


	Vertex* Triangle::operator[](int index) const {
		return pts[index];
	}


	bool Triangle::clip_f(Vec3f* tri, const Vec3f& color_e) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
			float c2 = (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);

			// new tri formed from quad
			Vec3f temp[3] = { 0.0f };
			Vec3f temp_c[3] = { 0.0f };

			// interpolate x and y
			temp[0].x = tri[1].x + (tri[0].x - tri[1].x) * c1;
			temp[0].y = tri[1].y + (tri[0].y - tri[1].y) * c1;
			temp[0].z = RenderBL::znear;
			temp[1].x = tri[1].x + (tri[2].x - tri[1].x) * c2;
			temp[1].y = tri[1].y + (tri[2].y - tri[1].y) * c2;
			temp[1].z = RenderBL::znear;
			temp[2] = tri[2];

			// update original tri
			tri[1] = temp[0];

			// raster new tri
			raster_f(temp, color_e);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
			float c2 = (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);

			// interpolate x and y
			tri[0].x = tri[0].x + (tri[1].x - tri[0].x) * c1;
			tri[0].y = tri[0].y + (tri[1].y - tri[0].y) * c1;
			tri[0].z = RenderBL::znear;
			tri[2].x = tri[2].x + (tri[1].x - tri[2].x) * c2;
			tri[2].y = tri[2].y + (tri[1].y - tri[2].y) * c2;
			tri[2].z = RenderBL::znear;
		};

		if (tri[0].z < RenderBL::znear) {
			if (tri[1].z < RenderBL::znear) {
				// case 2
				if (tri[2].z < RenderBL::znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tri[1], tri[2]);
					clip2();
				}
			}
			// case 8
			else if (tri[2].z < RenderBL::znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tri[1], tri[0]);
				clip1();
			}
		}

		else if (tri[1].z < RenderBL::znear) {
			// case 7
			if (tri[2].z < RenderBL::znear) {
				std::swap(tri[1], tri[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tri[2].z < RenderBL::znear) {
			std::swap(tri[1], tri[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	bool Triangle::clip_v(Vec3f* tri, Vec3f* tri_c) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
			float c2 = (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);

			// new tri formed from quad
			Vec3f temp[3] = { 0.0f };
			Vec3f temp_c[3] = { 0.0f };

			// interpolate x and y
			temp[0].x = tri[1].x + (tri[0].x - tri[1].x) * c1;
			temp[0].y = tri[1].y + (tri[0].y - tri[1].y) * c1;
			temp[0].z = RenderBL::znear;
			temp[1].x = tri[1].x + (tri[2].x - tri[1].x) * c2;
			temp[1].y = tri[1].y + (tri[2].y - tri[1].y) * c2;
			temp[1].z = RenderBL::znear;
			temp[2] = tri[2];

			// interpolate color
			temp_c[0] = tri_c[1] + (tri_c[0] - tri_c[1]) * c1;
			temp_c[1] = tri_c[1] + (tri_c[2] - tri_c[1]) * c2;
			temp_c[2] = tri_c[2];

			// update original tri
			tri[1] = temp[0];
			tri_c[1] = temp_c[0];

			// raster new tri
			raster_v(temp, temp_c);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
			float c2 = (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);

			// interpolate x and y
			tri[0].x = tri[0].x + (tri[1].x - tri[0].x) * c1;
			tri[0].y = tri[0].y + (tri[1].y - tri[0].y) * c1;
			tri[0].z = RenderBL::znear;
			tri[2].x = tri[2].x + (tri[1].x - tri[2].x) * c2;
			tri[2].y = tri[2].y + (tri[1].y - tri[2].y) * c2;
			tri[2].z = RenderBL::znear;

			// interpolate color
			tri_c[0] = tri_c[0] + (tri_c[1] - tri_c[0]) * c1;
			tri_c[2] = tri_c[2] + (tri_c[1] - tri_c[2]) * c2;
		};

		if (tri[0].z < RenderBL::znear) {
			if (tri[1].z < RenderBL::znear) {
				// case 2
				if (tri[2].z < RenderBL::znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tri[1], tri[2]);
					std::swap(tri_c[1], tri_c[2]);
					clip2();
				}
			}
			// case 8
			else if (tri[2].z < RenderBL::znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tri[1], tri[0]);
				std::swap(tri_c[1], tri_c[0]);
				clip1();
			}
		}

		else if (tri[1].z < RenderBL::znear) {
			// case 7
			if (tri[2].z < RenderBL::znear) {
				std::swap(tri[1], tri[0]);
				std::swap(tri_c[1], tri_c[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tri[2].z < RenderBL::znear) {
			std::swap(tri[1], tri[2]);
			std::swap(tri_c[1], tri_c[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	void Triangle::raster_f(Vec3f* tri, const Vec3f& color_e) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = RenderBL::cam.getScreenCoord(tri[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0].y > triScreen[1].y) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tri[0], tri[1]);
		}
		if (triScreen[0].y > triScreen[2].y) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tri[0], tri[2]);
		}
		if (triScreen[1].y > triScreen[2].y) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tri[1], tri[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2].y < 0 || triScreen[0].y > RenderBL::size.y) return;

		// get color
		Vec3f pos = (pts[0]->pos + pts[1]->pos + pts[2]->pos) / 3.0f;
		Vertex v = { pos, getNormal(), RenderBL::light_ambient };
		for (auto& l : RenderBL::lights) {
			l->getLight(v);
		}
		v.color.x = std::min(v.color.x, 255.0f);
		v.color.y = std::min(v.color.y, 255.0f);
		v.color.z = std::min(v.color.z, 255.0f);
		v.color -= color_e;
		v.color.x = std::max(v.color.x, RenderBL::light_ambient.x);
		v.color.y = std::max(v.color.y, RenderBL::light_ambient.y);
		v.color.z = std::max(v.color.z, RenderBL::light_ambient.z);
		int color = rgbToDec(v.color);

		// change in x per change in y
		float dx1_y = (float)(triScreen[2].x - triScreen[0].x) / (float)(triScreen[2].y - triScreen[0].y);
		float dx2_y = (float)(triScreen[1].x - triScreen[0].x) / (float)(triScreen[1].y - triScreen[0].y);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tri[2].z - tri[0].z) / (float)(triScreen[2].y - triScreen[0].y);
		float dz2_y = (tri[1].z - tri[0].z) / (float)(triScreen[1].y - triScreen[0].y);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// always ensure x1 < x2
		if (dx1_y > dx2_y) {
			std::swap(dx1_y, dx2_y);
			std::swap(dz1_y, dz2_y);
			dx_y = &dx1_y;
			dz_y = &dz1_y;
		}

		// y value of scanline
		int y = std::max(0, triScreen[0].y);
		int ymax = std::min(triScreen[1].y, RenderBL::size.y);

		// start and end x values of scanline
		float x1 = (float)triScreen[0].x + dx1_y * (float)(y - triScreen[0].y);
		float x2 = (float)triScreen[0].x + dx2_y * (float)(y - triScreen[0].y);

		// start and end z values of scanline
		float z1 = tri[0].z + dz1_y * (float)(y - triScreen[0].y);
		float z2 = tri[0].z + dz2_y * (float)(y - triScreen[0].y);

		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, RenderBL::size.x);
				float dz_x = (z2 - z1) / (x2 - x1);
				float z = z1 + dz_x * (x - (int)x1);
				int index = RenderBL::coordsToIndex({ x, y });
				for (x; x < xmax; x++) {
					if (z < RenderBL::depth[index]) {
						RenderBL::depth[index] = z;
						RenderBL::pixels[index] = color;
					}
					index++;
					z += dz_x;
				}
				x1 += dx1_y;
				x2 += dx2_y;
				z1 += dz1_y;
				z2 += dz2_y;
			}
		};

		// draw first tri segment
		rasterSegment();

		// update values for second segment
		ymax = std::min(triScreen[2].y, RenderBL::size.y);

		*dx_y = (float)(triScreen[2].x - triScreen[1].x) / (float)(triScreen[2].y - triScreen[1].y);
		x1 = (float)triScreen[2].x - dx1_y * (float)(triScreen[2].y - y);
		x2 = (float)triScreen[2].x - dx2_y * (float)(triScreen[2].y - y);

		*dz_y = (tri[2].z - tri[1].z) / (float)(triScreen[2].y - triScreen[1].y);
		z1 = tri[2].z - dz1_y * (float)(triScreen[2].y - y);
		z2 = tri[2].z - dz2_y * (float)(triScreen[2].y - y);

		// draw second tri segment
		rasterSegment();
	}


	void Triangle::raster_v(Vec3f* tri, Vec3f* tri_c) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = RenderBL::cam.getScreenCoord(tri[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0].y > triScreen[1].y) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tri[0], tri[1]);
			std::swap(tri_c[0], tri_c[1]);
		}
		if (triScreen[0].y > triScreen[2].y) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tri[0], tri[2]);
			std::swap(tri_c[0], tri_c[2]);
		}
		if (triScreen[1].y > triScreen[2].y) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tri[1], tri[2]);
			std::swap(tri_c[1], tri_c[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2].y < 0 || triScreen[0].y > RenderBL::size.y) return;

		// change in x per change in y
		float dx1_y = (float)(triScreen[2].x - triScreen[0].x) / (float)(triScreen[2].y - triScreen[0].y);
		float dx2_y = (float)(triScreen[1].x - triScreen[0].x) / (float)(triScreen[1].y - triScreen[0].y);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tri[2].z - tri[0].z) / (float)(triScreen[2].y - triScreen[0].y);
		float dz2_y = (tri[1].z - tri[0].z) / (float)(triScreen[1].y - triScreen[0].y);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// change in color per change in y
		Vec3f dc1_y = (tri_c[2] - tri_c[0]) / (float)(triScreen[2].y - triScreen[0].y);
		Vec3f dc2_y = (tri_c[1] - tri_c[0]) / (float)(triScreen[1].y - triScreen[0].y);
		Vec3f* dc_y = &dc2_y;	// points to dc for side of triangle with pt1 (2 segments)

		// always ensure x1 < x2
		if (dx1_y > dx2_y) {
			std::swap(dx1_y, dx2_y);
			std::swap(dz1_y, dz2_y);
			std::swap(dc1_y, dc2_y);
			dx_y = &dx1_y;
			dz_y = &dz1_y;
			dc_y = &dc1_y;
		}

		// y value of scanline
		int y = std::max(0, triScreen[0].y);
		int ymax = std::min(triScreen[1].y, RenderBL::size.y);

		// start and end x values of scanline
		float x1 = (float)triScreen[0].x + dx1_y * (float)(y - triScreen[0].y);
		float x2 = (float)triScreen[0].x + dx2_y * (float)(y - triScreen[0].y);

		// start and end z values of scanline
		float z1 = tri[0].z + dz1_y * (float)(y - triScreen[0].y);
		float z2 = tri[0].z + dz2_y * (float)(y - triScreen[0].y);

		// start and end color values of scanline
		Vec3f c1 = tri_c[0] + dc1_y * (float)(y - triScreen[0].y);
		Vec3f c2 = tri_c[0] + dc2_y * (float)(y - triScreen[0].y);

		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, RenderBL::size.x);
				float dz_x = (z2 - z1) / (x2 - x1);
				float z = z1 + dz_x * (x - (int)x1);
				Vec3f dc_x = (c2 - c1) / (x2 - x1);
				Vec3f c = c1 + dc_x * (x - x1);
				int index = RenderBL::coordsToIndex({ x, y });
				for (x; x < xmax; x++) {
					if (z < RenderBL::depth[index]) {
						RenderBL::depth[index] = z;
						RenderBL::pixels[index] = rgbToDec(c);
					}
					index++;
					z += dz_x;
					c += dc_x;
				}
				x1 += dx1_y;
				x2 += dx2_y;
				z1 += dz1_y;
				z2 += dz2_y;
				c1 += dc1_y;
				c2 += dc2_y;
			}
		};

		// draw first tri segment
		rasterSegment();

		// update values for second segment
		ymax = std::min(triScreen[2].y, RenderBL::size.y);

		*dx_y = (float)(triScreen[2].x - triScreen[1].x) / (float)(triScreen[2].y - triScreen[1].y);
		x1 = (float)triScreen[2].x - dx1_y * (float)(triScreen[2].y - y);
		x2 = (float)triScreen[2].x - dx2_y * (float)(triScreen[2].y - y);

		*dz_y = (tri[2].z - tri[1].z) / (float)(triScreen[2].y - triScreen[1].y);
		z1 = tri[2].z - dz1_y * (float)(triScreen[2].y - y);
		z2 = tri[2].z - dz2_y * (float)(triScreen[2].y - y);

		*dc_y = (tri_c[2] - tri_c[1]) / (float)(triScreen[2].y - triScreen[1].y);
		c1 = tri_c[2] - dc1_y * (float)(triScreen[2].y - y);
		c2 = tri_c[2] - dc2_y * (float)(triScreen[2].y - y);

		// draw second tri segment
		rasterSegment();
	}

}
