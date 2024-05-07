#pragma once

#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>
#include <vector>

namespace tigl
{
	namespace internal
	{
		class Shader
		{
		public:
			virtual ~Shader() {};
			
			// Sets the projection matrix
			virtual void setProjectionMatrix(const glm::mat4& matrix) = 0;

			// Sets the view (camera) matrix
			virtual void setViewMatrix(const glm::mat4& matrix) = 0;

			// Sets the model matrix
			virtual void setModelMatrix(const glm::mat4& matrix) = 0;

			// enables the use of the colors set in vertices
			virtual void enableColor(bool enabled) = 0;

			// enables the use of texture coordinats set in vertices, and uses textures set in texture sampler
			virtual void enableTexture(bool enabled) = 0;
			
			// enables the lighting
			virtual void enableLighting(bool enabled) = 0;

			// sets the number of lights
			virtual void setLightCount(int count) = 0;

			// sets the light as directional or positional
			virtual void setLightDirectional(int lightNr, bool isDirectional) = 0;

			// sets the position of the light. If the light is a directional light, the position is interpreted as a direction
			virtual void setLightPosition(int lightNr, const glm::vec3& position) = 0;

			// sets the ambient color of a light
			virtual void setLightAmbient(int lightNr, const glm::vec3& color) = 0;

			// sets the diffuse color of a light
			virtual void setLightDiffuse(int lightNr, const glm::vec3& color) = 0;

			// sets the specular color of a light
			virtual void setLightSpecular(int lightNr, const glm::vec3& color) = 0;

			// sets the shinyness of the material drawn. Used for specular calculations
			virtual void setShinyness(float shinyness) = 0;

			// Enables color multiplication. If enabled, all colors (texture and vertex colors) will be multiplied by the color set by the setColorMult method
			virtual void enableColorMult(bool enabled) = 0;

			// Changes the color that output will be multiplied with when enableColorMult is enabled
			virtual void setColorMult(const glm::vec4& color) = 0;

			// Enables alpha testing. Will stop rendering everything with a low alpha value
			virtual void enableAlphaTest(bool enabled) = 0;

			// Enables fog
			virtual void enableFog(bool enabled) = 0;

			// Sets the fog to linear
			virtual void setFogLinear(float begin, float end) = 0;

			// Sets the fog to Exponential
			virtual void setFogExp(float density) = 0;

			// Sets the fog to Exponential
			virtual void setFogExp2(float density) = 0;

			// Sets the color of the fog
			virtual void setFogColor(const glm::vec3 &color) = 0;
		};
	}
	// A simple structure to store vertices. Can store positions, normals, colors and texture coordinats
	struct Vertex
	{
	public:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 texcoord;

		// Creates a vertex with a position
		static Vertex P(const glm::vec3& position) {
			return { position, glm::vec3(0,1,0), glm::vec4(1,1,1,1), glm::vec2(0,0) };
		}

		// Creates a vertex with a position and a color
		static Vertex PC(const glm::vec3& position, const glm::vec4& color) {
			return { position, glm::vec3(0,1,0), color, glm::vec2(0,0) };
		}

		// Creates a vertex with a position and a texture coordinat
		static Vertex PT(const glm::vec3& position, const glm::vec2& texcoord) {
			return { position, glm::vec3(0,1,0), glm::vec4(1,1,1,1), texcoord };
		}

		// Creates a vertex with a position and a normal
		static Vertex PN(const glm::vec3& position, const glm::vec3& normal) {
			return { position, normal, glm::vec4(1,1,1,1), glm::vec2(0,0) };
		}

		// Creates a vertex with a position, a texture coordinat and a color
		static Vertex PTC(const glm::vec3& position, const glm::vec2& texcoord, const glm::vec4 &color) {
			return { position, glm::vec3(0,1,0), color, texcoord };
		}


		// Creates a vertex with a position, color and normal
		static Vertex PCN(const glm::vec3& position, const glm::vec4& color, const glm::vec3& normal) {
			return { position, normal, color, glm::vec2(0,0) };
		}

		// Creates a vertex with a position, texture coordinat and normal
		static Vertex PTN(const glm::vec3& position, const glm::vec2& texcoord, const glm::vec3& normal) {
			return { position, normal, glm::vec4(1,1,1,1), texcoord };
		}

		// Creates a vertex with a position, color, texture coordinat and normal
		static Vertex PCTN(const glm::vec3& position, const glm::vec4& color, const glm::vec2& texcoord, const glm::vec3& normal) {
			return { position, normal, color, texcoord };
		}

		bool operator == (const Vertex& other);
	};

	class VBO
	{
	public:
		~VBO();
	private:
		GLuint id;
		unsigned int size;
		friend void drawVertices(GLenum shape, VBO* vbo);
		friend VBO* createVbo(const std::vector<Vertex>& vertices);
	};

	// Access point for the shader
	extern std::unique_ptr<internal::Shader> shader;

	// Call to initialize. Loads the shader
	void init();

	// Call to start a set of OpenGL primitives
	void begin(GLenum shape);

	// Adds a single vertex to the set
	void addVertex(const Vertex& vertex);

	// Finishes and draws the vertices given
	void end();

	// Draws a full array of vertices
	void drawVertices(GLenum shape, const std::vector<Vertex> &vertices);

	// Creates a VBO
	VBO* createVbo(const std::vector<Vertex> &vertices);

	// draws vertices from a VBO
	void drawVertices(GLenum shape, VBO* vbo);
}



