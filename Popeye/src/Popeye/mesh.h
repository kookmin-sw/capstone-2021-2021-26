#pragma once

namespace Popeye {
	class mesh
	{
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
	public:
		mesh();
		~mesh();
		void init_buffer(float*, int, unsigned int*, int);
		void draw_mesh();
	};
}

