#include "MultiLine.h"

#include <iostream>
#include <cmath>

namespace cgCourse
{
	MultiLine::MultiLine(const std::vector<glm::vec3> & _vertices,
						 const std::vector<glm::vec3> & _normals
						 ) : Shape()

{
    // TODO: initialize positions, colors, and normals
    // initialize positions
    float length = 0.1;

    for (int i = 0; i < _vertices.size(); i++){
        positions.push_back(_vertices[i]);
        positions.push_back(_vertices[i] + length * _normals[i]);

        colors.push_back({4.0f, 10.8f, 0.5f});
        colors.push_back({4.0f, 10.8f, 0.5f});

        lineIndices.push_back({2*i, 2*i+1});
    }
}
	void MultiLine::draw() const
	{
		// TODO: complete code to draw the normals as lines
        // bind the Vertex Array Object and draw the lines
                glBindVertexArray(vaoID);
                glDrawElements(GL_LINES,  lineIndices.size() * 2, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
	}

void MultiLine::initIndexBuffer() {
        // Initialize buffer objects with index data
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIndices.size() * sizeof(glm::uvec2),
                     lineIndices.data(), GL_STATIC_DRAW);
    }
}
    
