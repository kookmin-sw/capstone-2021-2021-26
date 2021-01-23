#pragma once

namespace Popeye {
	class mesh
	{
		unsigned int VBO;
		unsigned int VAO;
	public:
		mesh();
		~mesh();
		void init_buffer(float* vertices, int size);
		void draw_mesh();
	};
}

