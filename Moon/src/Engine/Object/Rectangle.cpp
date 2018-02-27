#include "include/Engine/Object/Rectangle.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include "include/Engine/GameHandler.h"

using namespace Moon;

//Constructor
Object::Rectangle::Rectangle()
{
	DEFINE_OBJECT_CONSTRUCTOR("Rectangle");
	this->_size = Physics::Vector2(100.0, 100.0);
	this->_color = Graphics::Color3(1.0f, 1.0f, 1.0f);
}

//Deconstructor
Object::Rectangle::~Rectangle()
{
	
}

//Member Getters
Physics::Vector2 Object::Rectangle::GetSize() const
{
	return this->_size;
}

//Member Setters
void Object::Rectangle::SetSize(Physics::Vector2 value)
{
	this->_size = value;
}

//Methods
void Object::Rectangle::Render(GLuint shaderId, GLuint matrixId)
{
	GameHandler* game = GameHandler::singleton();

	Physics::Vector2 position = this->GetPosition();
	Physics::Vector2 size = this->GetSize();
	Graphics::Color3 color = this->GetColor();

	//Projection
	glm::mat4 worldSpaceMatrix = glm::translate(glm::mat4(), glm::vec3((float)position.GetX(), (float)position.GetY(), 0.0f));
	glm::mat4 cameraSpaceMatrix = game->GetCameraMatrix();
	glm::mat4 projectionMatrix = game->GetProjectionMatrix();
	glm::mat4 worldViewMatrix = projectionMatrix * cameraSpaceMatrix * worldSpaceMatrix;
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &worldViewMatrix[0][0]);

	//Vertices for rectangle (at origin)
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, (float)size.GetY(), 0,
		(float)size.GetX(), (float)size.GetY(), 0,

		0.0f, 0.0f, 0.0f,
		(float)size.GetX(), (float)size.GetY(), 0,
		(float)size.GetX(), 0, 0
	};

	//Colors
	GLfloat colors[] = {
		color.GetRed(), color.GetGreen(), color.GetBlue(),
		color.GetRed(), color.GetGreen(), color.GetBlue(),
		color.GetRed(), color.GetGreen(), color.GetBlue(),

		color.GetRed(), color.GetGreen(), color.GetBlue(),
		color.GetRed(), color.GetGreen(), color.GetBlue(),
		color.GetRed(), color.GetGreen(), color.GetBlue()
	};

	//Texture
	GLfloat texCoords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Send vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Send color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->_colorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Send texture
	glEnableVertexAttribArray(2);
	static GLuint hasTextureId = glGetUniformLocation(shaderId, "hasTexture");
	int hasTexture = 0;
	Content textureContent = ContentProvider::singleton()->Get(this->_textureContentId);
	if (textureContent.type != ContentType::NullContent)
	{
		if (!game->IsWireframe())
		{
			glBindTexture(GL_TEXTURE_2D, textureContent.gl_TextureId);
			hasTexture = 1;
		}
	}
	if (hasTextureId != -1)
	{
		glUniform1i(hasTextureId, hasTexture);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->_textureBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Draw
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
void Object::Rectangle::StepPhysics(double frameDeltaSec)
{
	if (this->IsAnchored()) { return; }
	Physics::Vector2 velocity = this->GetVelocity();
	Physics::Vector2 position = this->GetPosition();
	double rotVelocity = this->GetRotVelocity();
	double rotation = this->GetRotation();
	if (velocity.GetMagnitude() != 0.0)
	{
		double xPosition = position.GetX();
		double yPosition = position.GetY();
		double xVelocity = velocity.GetX();
		double yVelocity = velocity.GetY();
		this->SetPosition(Physics::Vector2(
			xPosition + (xVelocity*frameDeltaSec), 
			yPosition + (yVelocity*frameDeltaSec)
		));
	}
	if (this->GetRotVelocity() != 0.0)
	{
		this->SetRotation(rotation + (rotVelocity*frameDeltaSec));
	}
}