#include "Torus.h"

#include <iostream>
#include <cmath>

namespace cgCourse
{
	// a simple 2D circle
	class Circle
	{
	public:
		Circle(float _radius, int _segments)
		{
			radius = _radius;
			segments = _segments;

			for (int j = 0; j < segments; j++)
			{
				float angle = (float) j * (360.0 / segments);
				float radians = Circle::toRadians(angle);
				vertices.push_back(glm::vec2(radius * std::sin(radians), radius * std::cos(radians)));
			}
		}

		const std::vector<glm::vec2> & getVertices() const
		{
			return vertices;
		}

		float getRadius() const
		{
			return radius;
		}

		int getSegments() const
		{
			return segments;
		}

		static float toRadians(float _angle)
		{
			return _angle * M_PI / 180.0;
		}

	private:
		float radius;
		int segments;

		std::vector<glm::vec2> vertices;
	};


	Torus::Torus() : Shape()
	{
		float radiusX = 1.0;		// radius in xz-plane (main radius)
		float radiusY = 0.3;		// radius in xy-plane (radius of the ring segments)
		int segmentsInXZPlane = 50;	// subdivision in xz-plane (amount of ring segments)
		int segmentsInXYPlane = 30;	// subdivision in xy-plane

		std::vector<glm::vec3> midPoints;
		std::vector<float> midPointsLenght;

		Circle circleXZ(radiusX, segmentsInXZPlane);
		Circle circleXY(radiusY, segmentsInXYPlane);

		// calculate positions
		for (int j = 0; j < circleXZ.getSegments(); j++)
		{
			for (int i = 0; i < circleXY.getVertices().size(); i++)
			{
				glm::vec3 currentPoint(circleXZ.getVertices()[j].x, 0.0, circleXZ.getVertices()[j].y);
				glm::vec3 dirExtension = glm::normalize(currentPoint) * circleXY.getVertices()[i].x;
				currentPoint.y = circleXY.getVertices()[i].y;
				positions.push_back(currentPoint + dirExtension);
			}
		}

		// calculate faces
		for(int j = 0; j < circleXZ.getSegments(); j++)
		{
			int startIdx = j * circleXY.getSegments();
			if(j + 1 == circleXZ.getSegments())
			{
				// special case for connecting the last ring segments
				for(int i = 0; i < circleXY.getSegments(); i++)
				{
					int currIdx = startIdx + i;
					int firstSegIdx = i;
					i + 1 == circleXY.getSegments() ?
									addTwoFaces(currIdx, firstSegIdx, startIdx, 0) :
									addTwoFaces(currIdx, firstSegIdx, currIdx + 1, firstSegIdx + 1);
				}
			}
			else
			{
				// this parts connects all ring segments
				for (int i = 0; i < circleXY.getSegments(); i++)
				{
					int currIdx = startIdx + i;
					
					// special case for the last two faces of a single ring segment
					i + 1 == circleXY.getSegments() ?
									addTwoFaces(currIdx, currIdx + circleXY.getSegments(), startIdx, startIdx + circleXY.getSegments()) :
									addTwoFaces(currIdx, currIdx + circleXY.getSegments(), currIdx + 1, currIdx + 1 + circleXY.getSegments());
				}
			}
		}
        


        
//        for (int i = 0; i < positions.size(); i++) {
//            float t = i / float(positions.size());
//
//                glm::vec4 c = glm::vec4(1.0, 1.0, 1.0, 1.0);
//                if (t < 0.25) {
//                    c.r = 0.0;
//                    c.g = t * 4.0;
//                } else if (t < 0.5) {
//                    c.r = 0.0;
//                    c.b = (0.5 - t) * 4.0;
//                } else if (t < 0.75) {
//                    c.r = (t - 0.5) * 4.0;
//                    c.b = 0.0;
//                } else {
//                    c.g = (1.0 - t) * 4.0;
//                    c.b = 0.0;
//                }
//            glm::vec4 color = c;
//            colors.push_back(color);
//        }
        // calculate positions and normals
        for (int j = 0; j < circleXZ.getSegments(); j++) {
            for (int i = 0; i < circleXY.getVertices().size(); i++) {
                glm::vec3 currentPoint(circleXZ.getVertices()[j].x, 0.0, circleXZ.getVertices()[j].y);
                glm::vec3 dirExtension = glm::normalize(currentPoint) * circleXY.getVertices()[i].x;
                currentPoint.y = circleXY.getVertices()[i].y;
                positions.push_back(currentPoint + dirExtension);
                normals.push_back(glm::normalize(currentPoint + dirExtension));
            }
        }

        // assign colors based on normals
        for (int i = 0; i < normals.size(); i++) {
            colors.push_back(glm::vec3((normals[i].x + 1.0f) / 2.0f, (normals[i].y + 1.0f) / 2.0f, (normals[i].z + 1.0f) / 2.0f));
        }
        
        
        
		// just color the object from black to green
//		colors.assign(positions.size(), glm::vec3(1.33, 0.55, 0.31));
        
//        colors.resize(positions.size());
//        for (int i = 0; i < positions.size(); i++) {
//            if (positions[i].y > 0) {
//                colors[i] = glm::vec3(1.0f, 0.0f, 0.0f); // red
//            } else {
//                colors[i] = glm::vec3(0.0f, 0.0f, 1.0f); // blue
//            }
//        }
//
//
        
        
        
        
        
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), &colors[0], GL_STATIC_DRAW);

		// adding normals
		normals.resize(positions.size());
		for(const auto & f: faces)
		{
			glm::vec3 v1 = positions[f.y] - positions[f.x];
			glm::vec3 v2 = positions[f.z] - positions[f.x];
			glm::vec3 n = glm::normalize(glm::cross(v1, v2));
			normals[f.x] += n;
			normals[f.y] += n;
			normals[f.z] += n;
		}

		for(auto & n: normals)
			n = glm::normalize(n);
	}


	void Torus::addTwoFaces(unsigned int _idx0, unsigned int _idx1, unsigned int _idx2, unsigned int _idx3)
	{
		faces.push_back(glm::uvec3(_idx0, _idx2, _idx1));
		faces.push_back(glm::uvec3(_idx2, _idx3, _idx1));
	}
}

