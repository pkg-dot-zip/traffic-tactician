#include "tigl.h"

#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

namespace tigl
{
	class ShaderImpl : public internal::Shader
	{
	public:
		ShaderImpl();
		~ShaderImpl();
		void use();

		void setProjectionMatrix(const glm::mat4& matrix);
		void setViewMatrix(const glm::mat4& matrix);
		void setModelMatrix(const glm::mat4& matrix);

		void enableColor(bool enabled) { setUniform(Uniform::useColor, enabled); }
		void enableTexture(bool enabled) { setUniform(Uniform::useTexture, enabled); }
		void enableLighting(bool enabled) { setUniform(Uniform::useLighting, enabled); }
		void setLightCount(int count) { setUniform(Uniform::lightCount, count); }
		void setLightDirectional(int lightNr, bool isDirectional) { setUniform("lights[" + std::to_string(lightNr) + "].directional", isDirectional); }
		void setLightPosition(int lightNr, const glm::vec3& position) { setUniform("lights[" + std::to_string(lightNr) + "].position", position); }
		void setLightAmbient(int lightNr, const glm::vec3& color) { setUniform("lights[" + std::to_string(lightNr) + "].ambient", color); }
		void setLightDiffuse(int lightNr, const glm::vec3& color) { setUniform("lights[" + std::to_string(lightNr) + "].diffuse", color); }
		void setLightSpecular(int lightNr, const glm::vec3& color) { setUniform("lights[" + std::to_string(lightNr) + "].specular", color); }
		void setShinyness(float shinyness) { setUniform(Uniform::shinyness, shinyness); }

		void enableColorMult(bool enabled) { setUniform(Uniform::useColorMult, enabled); }
		void setColorMult(const glm::vec4& color) { setUniform(Uniform::colorMult, color); }

		void enableAlphaTest(bool enabled) { setUniform(Uniform::useAlphaTest, enabled); }

		void enableFog(bool enabled) { setUniform(Uniform::useFog, enabled); }
		void setFogLinear(float begin, float end) {
			setUniform(Uniform::fogType, 0);
			setUniform(Uniform::fogLinNear, begin);
			setUniform(Uniform::fogLinFar, end);
		}
		void setFogExp(float density) {
			setUniform(Uniform::fogType, 1);
			setUniform(Uniform::fogExpDensity, density);
		}
		void setFogExp2(float density) {
			setUniform(Uniform::fogType, 2);
			setUniform(Uniform::fogExpDensity, density);
		}
		void setFogColor(const glm::vec3& color) { setUniform(Uniform::fogColor, color); }

	private:
		void addShader(int shaderProgram, GLenum shaderType, const std::string& shader);
		GLuint programId;
		enum Uniform
		{
			//matrices
			ProjectionMatrix,
			ViewMatrix,
			ModelMatrix,
			NormalMatrix,
			//flags
			useColor,
			useColorMult,
			useTexture,
			useLighting,
			useAlphaTest,
			useFog,
			//parameters
			colorMult,
			lightCount,
			shinyness,
			cameraPosition,
			fogType,
			fogLinNear,
			fogLinFar,
			fogExpDensity,
			fogColor,


			UniformMax
		};

		int uniforms[UniformMax];

		void setUniform(Uniform uniform, const glm::mat4& value);
		void setUniform(Uniform uniform, const glm::mat3& value);
		void setUniform(Uniform uniform, const glm::vec4& value);
		void setUniform(Uniform uniform, const glm::vec3& value);
		void setUniform(Uniform uniform, const glm::vec2& value);
		void setUniform(Uniform uniform, float value);
		void setUniform(Uniform uniform, int value);
		void setUniform(Uniform uniform, bool value);

		//slower
		void setUniform(const std::string& uniform, const glm::mat4& value);
		void setUniform(const std::string& uniform, const glm::mat3& value);
		void setUniform(const std::string& uniform, const glm::vec4& value);
		void setUniform(const std::string& uniform, const glm::vec3& value);
		void setUniform(const std::string& uniform, const glm::vec2& value);
		void setUniform(const std::string& uniform, float value);
		void setUniform(const std::string& uniform, int value);
		void setUniform(const std::string& uniform, bool value);

		glm::mat4 modelMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

	};

	std::unique_ptr<internal::Shader> shader;
	int attributePosition = 0;
	int attributeColor = 1;
	int attributeTexcoord = 2;
	int attributeNormal = 3;


	// Initializes shader used
	void init()
	{
		glewInit();
		shader.reset(new ShaderImpl());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}

	std::vector<Vertex> vertices;
	GLenum shape = 0;

	void begin(GLenum shape)
	{
		assert(tigl::shape == 0);
		tigl::shape = shape;
		vertices.clear();
	}
	void addVertex(const Vertex& vertex)
	{
		vertices.push_back(vertex);
	}
	void end()
	{
		assert(shape != 0);
		drawVertices(shape, vertices);
		shape = 0;
	}

	void drawVertices(GLenum shape, const std::vector<Vertex>& vertices)
	{
		if (vertices.size() > 0)
		{
			glVertexAttribPointer(tigl::attributePosition, 3, GL_FLOAT, false, sizeof(Vertex), &vertices[0].position);
			glVertexAttribPointer(tigl::attributeColor, 4, GL_FLOAT, false, sizeof(Vertex), &vertices[0].color);
			glVertexAttribPointer(tigl::attributeTexcoord, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoord);
			glVertexAttribPointer(tigl::attributeNormal, 3, GL_FLOAT, false, sizeof(Vertex), &vertices[0].normal);
			glDrawArrays(shape, 0, (GLsizei)vertices.size());
		}
	}

	VBO* createVbo(const std::vector<Vertex>& vertices)
	{
		VBO* vbo = new VBO();
		glGenBuffers(1, &vbo->id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		vbo->size = (unsigned int)vertices.size();
		return vbo;
	}
	VBO::~VBO()
	{
		glDeleteBuffers(1, &id);
	}


	void drawVertices(GLenum shape, VBO* vbo)
	{
		static Vertex tmpVertex;
		if (vbo->size > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo->id);

			glVertexAttribPointer(tigl::attributePosition, 3, GL_FLOAT, false, sizeof(Vertex), (void*)((char*)&tmpVertex.position - (char*)&tmpVertex));
			glVertexAttribPointer(tigl::attributeColor, 4, GL_FLOAT, false, sizeof(Vertex), (void*)((char*)&tmpVertex.color - (char*)&tmpVertex));
			glVertexAttribPointer(tigl::attributeTexcoord, 2, GL_FLOAT, false, sizeof(Vertex), (void*)((char*)&tmpVertex.texcoord - (char*)&tmpVertex));
			glVertexAttribPointer(tigl::attributeNormal, 3, GL_FLOAT, false, sizeof(Vertex), (void*)((char*)&tmpVertex.normal - (char*)&tmpVertex));
			glDrawArrays(shape, 0, (GLsizei)vbo->size);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}






	ShaderImpl::ShaderImpl()
	{
		this->programId = glCreateProgram();
		addShader(this->programId, GL_VERTEX_SHADER, R"ESC(#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec3 a_normal;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat3 normalMatrix = mat3(1.0);

out vec4 color;
out vec2 texCoord;
out vec3 normal;
out vec3 position;

void main()
{
	texCoord = a_texcoord;
	color = a_color;
	normal = normalMatrix * a_normal;
	position = vec3(modelMatrix * vec4(a_position,1));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}
)ESC");
		addShader(this->programId, GL_FRAGMENT_SHADER, R"ESC(#version 330
layout(location = 0) out vec4 fragColor;
uniform sampler2D s_texture;

//flags
uniform bool useColor = false;
uniform bool useColorMult = false;
uniform bool useTexture = false;
uniform bool useLighting = false;
uniform bool useAlphaTest = false;
uniform bool useFog = false;
//parameters
uniform vec4 colorMult = vec4(1,1,1,1);
uniform vec3 fogColor = vec3(1.0);
uniform vec3 cameraPosition;

uniform int fogType = 0;
uniform float fogLinNear = 0;
uniform float fogLinFar = 100;
uniform float fogExpDensity = 0;


uniform float shinyness = 0;
struct Light
{
	bool directional;
	vec3 position;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};
uniform Light lights[5];
uniform int lightCount = 1;


in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 position;


float fogFactorLinear(const float dist, const float start, const float end) {
  return 1.0 - clamp((end - dist) / (end - start), 0.0, 1.0);
}

float fogFactorExp2(const float dist, const float density) {
  const float LOG2 = -1.442695;
  float d = density * dist;
  return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}

float fogFactorExp(const float dist, const float density) {
  return 1.0 - clamp(exp(-density * dist), 0.0, 1.0);
}

void main()
{
	vec4 outputColor = vec4(1,1,1,1);
	if(useColor)
		outputColor *= color;
	if(useColorMult)
		outputColor *= colorMult;
	if(useTexture)
		outputColor *= texture2D(s_texture, texCoord);

	if(useLighting) {
		vec3 ambient = vec3(0,0,0);
		vec3 specular = vec3(0,0,0);
		vec3 diffuse = vec3(0,0,0);

		for(int i = 0; i < lightCount; i++) {
		
			vec3 lightDir = normalize(lights[i].position - position);
			if(lights[i].directional)
				lightDir = normalize(lights[i].position);

			ambient += lights[i].ambient;

			float diffuseFactor = max(0, dot(lightDir, normalize(normal)));
			diffuse += diffuseFactor * lights[i].diffuse;
		
			vec3 reflectDir = reflect(-lightDir, normalize(normal));
			float specularFactor = pow(max(dot(normalize(cameraPosition-position), reflectDir), 0.0), shinyness);
			if(shinyness > 0)
				specular += specularFactor * lights[i].specular;
		}
		
		outputColor.rgb = (ambient + specular + diffuse) * outputColor.rgb;
	}
	if(useFog) {
		float fogDistance = gl_FragCoord.z / gl_FragCoord.w;
		if(fogType == 0)
			outputColor.rgb = mix(outputColor.rgb, fogColor, fogFactorLinear(fogDistance, fogLinNear,fogLinFar));
		else if(fogType == 1)
			outputColor.rgb = mix(outputColor.rgb, fogColor, fogFactorExp(fogDistance, fogExpDensity));
		else if(fogType == 2)
			outputColor.rgb = mix(outputColor.rgb, fogColor, fogFactorExp2(fogDistance, fogExpDensity));

	}


	if(useAlphaTest && outputColor.a < 0.01)
		discard;
	fragColor = outputColor;
}
)ESC");
		glLinkProgram(programId);

		GLint status;
		glGetProgramiv(programId, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			int length, charsWritten;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
			char* infolog = new char[length + 1];
			memset(infolog, 0, length + 1);
			glGetProgramInfoLog(programId, length, &charsWritten, infolog);
			std::cout << "Error compiling shader:\n" << infolog << std::endl;
			delete[] infolog;
			return;
		}

		uniforms[Uniform::ModelMatrix] = glGetUniformLocation(programId, "modelMatrix");
		uniforms[Uniform::ViewMatrix] = glGetUniformLocation(programId, "viewMatrix");
		uniforms[Uniform::ProjectionMatrix] = glGetUniformLocation(programId, "projectionMatrix");
		uniforms[Uniform::NormalMatrix] = glGetUniformLocation(programId, "normalMatrix");
		uniforms[Uniform::useColor] = glGetUniformLocation(programId, "useColor");
		uniforms[Uniform::useColorMult] = glGetUniformLocation(programId, "useColorMult");
		uniforms[Uniform::useTexture] = glGetUniformLocation(programId, "useTexture");
		uniforms[Uniform::useLighting] = glGetUniformLocation(programId, "useLighting");
		uniforms[Uniform::lightCount] = glGetUniformLocation(programId, "lightCount");

		uniforms[Uniform::useAlphaTest] = glGetUniformLocation(programId, "useAlphaTest");
		uniforms[Uniform::useFog] = glGetUniformLocation(programId, "useFog");
		uniforms[Uniform::colorMult] = glGetUniformLocation(programId, "colorMult");
		uniforms[Uniform::cameraPosition] = glGetUniformLocation(programId, "cameraPosition");
		uniforms[Uniform::shinyness] = glGetUniformLocation(programId, "shinyness");
		uniforms[Uniform::fogType] = glGetUniformLocation(programId, "fogType");
		uniforms[Uniform::fogLinNear] = glGetUniformLocation(programId, "fogLinNear");
		uniforms[Uniform::fogLinFar] = glGetUniformLocation(programId, "fogLinFar");
		uniforms[Uniform::fogExpDensity] = glGetUniformLocation(programId, "fogExpDensity");
		uniforms[Uniform::fogColor] = glGetUniformLocation(programId, "fogColor");

		use();
	}

	ShaderImpl::~ShaderImpl()
	{

	}
	void ShaderImpl::use()
	{
		glUseProgram(programId);
	}

	void ShaderImpl::addShader(int shaderProgram, GLenum shaderType, const std::string& shader)
	{
		GLuint shaderId = glCreateShader(shaderType);
		const char* shaderSource = shader.c_str();
		glShaderSource(shaderId, 1, &shaderSource, NULL);
		glCompileShader(shaderId);
		GLint status;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			int length, charsWritten;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
			char* infolog = new char[length + 1];
			memset(infolog, 0, length + 1);
			glGetShaderInfoLog(shaderId, length, &charsWritten, infolog);
			std::cout << "Error compiling shader:\n" << infolog << std::endl;
			delete[] infolog;
			return;
		}
		glAttachShader(programId, shaderId);
	}


	void ShaderImpl::setProjectionMatrix(const glm::mat4& matrix)
	{
		this->projectionMatrix = matrix;
		setUniform(Uniform::ProjectionMatrix, matrix);
	}

	void ShaderImpl::setViewMatrix(const glm::mat4& matrix)
	{
		this->viewMatrix = matrix;
		setUniform(Uniform::ViewMatrix, matrix);

		glm::vec4 cameraPosition = glm::inverse(matrix) * glm::vec4(0, 0, 0, 1);
		setUniform(Uniform::cameraPosition, glm::vec3(cameraPosition));

	}

	void ShaderImpl::setModelMatrix(const glm::mat4& matrix)
	{
		this->modelMatrix = matrix;
		setUniform(Uniform::ModelMatrix, matrix);
		setUniform(Uniform::NormalMatrix, glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
	}






	void ShaderImpl::setUniform(Uniform uniform, const glm::mat4& value)
	{
		glUniformMatrix4fv(uniforms[uniform], 1, false, glm::value_ptr(value));
	}

	void ShaderImpl::setUniform(Uniform uniform, const glm::mat3& value)
	{
		glUniformMatrix3fv(uniforms[uniform], 1, false, glm::value_ptr(value));
	}

	void ShaderImpl::setUniform(Uniform uniform, const glm::vec4& value)
	{
		glUniform4fv(uniforms[uniform], 1, glm::value_ptr(value));
	}
	void ShaderImpl::setUniform(Uniform uniform, const glm::vec3& value)
	{
		glUniform3fv(uniforms[uniform], 1, glm::value_ptr(value));
	}
	void ShaderImpl::setUniform(Uniform uniform, const glm::vec2& value)
	{
		glUniform2fv(uniforms[uniform], 1, glm::value_ptr(value));
	}

	void ShaderImpl::setUniform(Uniform uniform, bool value)
	{
		glUniform1i(uniforms[uniform], value ? GL_TRUE : GL_FALSE);
	}
	void ShaderImpl::setUniform(Uniform uniform, int value)
	{
		glUniform1i(uniforms[uniform], value);
	}
	void ShaderImpl::setUniform(Uniform uniform, float value)
	{
		glUniform1f(uniforms[uniform], value);
	}



	void ShaderImpl::setUniform(const std::string& uniform, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(programId, uniform.c_str()), 1, false, glm::value_ptr(value));
	}

	void ShaderImpl::setUniform(const std::string& uniform, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(programId, uniform.c_str()), 1, false, glm::value_ptr(value));
	}

	void ShaderImpl::setUniform(const std::string& uniform, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(programId, uniform.c_str()), 1, glm::value_ptr(value));
	}
	void ShaderImpl::setUniform(const std::string& uniform, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(programId, uniform.c_str()), 1, glm::value_ptr(value));
	}
	void ShaderImpl::setUniform(const std::string& uniform, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(programId, uniform.c_str()), 1, glm::value_ptr(value));
	}


	void ShaderImpl::setUniform(const std::string& uniform, bool value)
	{
		glUniform1i(glGetUniformLocation(programId, uniform.c_str()), value ? GL_TRUE : GL_FALSE);
	}
	void ShaderImpl::setUniform(const std::string& uniform, int value)
	{
		glUniform1i(glGetUniformLocation(programId, uniform.c_str()), value);
	}
	void ShaderImpl::setUniform(const std::string& uniform, float value)
	{
		glUniform1f(glGetUniformLocation(programId, uniform.c_str()), value);
	}

	bool Vertex::operator==(const Vertex& other)
	{
		return position == other.position && normal == other.normal && color == other.color && texcoord == other.texcoord;
	}





}
