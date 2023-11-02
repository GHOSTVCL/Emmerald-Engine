#include "ModuleMesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include <vector>
#include "CompMesh.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{


}

std::vector<MeshData*> ModuleMesh::LoadMesh(const char* file_path)
{

	const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene != nullptr && scene->HasMeshes())
	{
	
		GameObject* _go;
		if (gototal == 0) {
			_go = new GameObject("GameObject");
		}
		else {
			std::string goname = "GameObject ";
			goname += std::to_string(gototal);
			_go = new GameObject(goname);
		}
		gototal++;

		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) {

			MeshData* temp = new MeshData();
			
			for (unsigned int o = 0; o < scene->mMeshes[i]->mNumVertices; o++)
			{
				Vertex tempvertex;
				float3 vector;
				vector.x = scene->mMeshes[i]->mVertices[o].x;
				vector.y = scene->mMeshes[i]->mVertices[o].y;
				vector.z = scene->mMeshes[i]->mVertices[o].z;
				tempvertex.Position = vector;
				LOG("New mesh with %d vertices", scene->mMeshes[i]->mNumVertices);

				if (scene->mMeshes[i]->HasNormals())
				{
					tempvertex.Normal.x = scene->mMeshes[i]->mNormals[o].x;
					tempvertex.Normal.y = scene->mMeshes[i]->mNormals[o].y;
					tempvertex.Normal.z = scene->mMeshes[i]->mNormals[o].z;
					
				}
				if (scene->mMeshes[i]->HasTextureCoords(0))
				{
					tempvertex.TexCoords.x = scene->mMeshes[i]->mTextureCoords[0][o].x;
					tempvertex.TexCoords.y = scene->mMeshes[i]->mTextureCoords[0][o].y;
					
				}
				else
				{
					tempvertex.TexCoords.x = 0.0f;
					tempvertex.TexCoords.y = 0.0f;
				}
				
				temp->ourVertex.push_back(tempvertex);
			}


			if (scene->mMeshes[i]->HasFaces())
			{
			
				temp->indices.resize(scene->mMeshes[i]->mNumFaces * 3);// assume each face is a triangle

				for (uint y = 0; y < scene->mMeshes[i]->mNumFaces; y++)
				{
					if (scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {

						memcpy(&temp->indices[y * 3], scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(unsigned int));

					}
				}
			}
			temp->textid = nullptr;
			ourMeshes.push_back(temp);
			ourMeshes.back()->InitBuffers();
			
			GameObject* go;
			std::string name = "Mesh";
			name += std::to_string(i);
			go = new GameObject(name);
			go->GetComponent<CompMesh>()->SetMesh(ourMeshes.back());
			go->GetComponent<CompMesh>()->name = ("Mesh%i", i);
			go->GetComponent<CompMesh>()->path = file_path;
			_go->AddChild(go);

		}
		

		if (App->scene->selectedGO->parent == nullptr) {
			App->scene->root->AddChild(_go);
		}
		else {
			App->scene->selectedGO->AddChild(_go);
		}

		aiReleaseImport(scene);


	}
	else {
		LOG("Error loading scene % s", file_path);
	}
	return ourMeshes;
}

ModuleMesh::~ModuleMesh()
{
}

update_status Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleMesh::CleanUp()
{
	return true;
}

void MeshData::Draw(GLuint checkers) {

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_COORD_ARRAY);
	//Bind Mesh
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);

	//Bind Textures
	if (textid != nullptr) {
		glBindTexture(GL_TEXTURE_2D, textid->textID);
	}
	else if(printcheckers){
		glBindTexture(GL_TEXTURE_2D, checkers);

	}
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);

	if (printVertexNormals) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		for (unsigned int i = 0; i < ourVertex.size(); i++) {
			const Vertex& vertex = ourVertex[i];
			const float3& position = vertex.Position;
			const float3& normal = vertex.Normal;

			glVertex3f(position.x, position.y, position.z);

			float scale = 0.1f; 
			glVertex3f(position.x + scale * normal.x, position.y + scale * normal.y, position.z + scale * normal.z);
		}

		glEnd();
	}
	if (printFaceNormals) {

		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		for (unsigned int i = 0; i < indices.size(); i += 3) {
			// Obtener los índices de los vértices de la cara
			unsigned int index1 = indices[i];
			unsigned int index2 = indices[i + 1];
			unsigned int index3 = indices[i + 2];

			// Calcular el punto medio de la cara (usamos la posición promedio de los vértices)
			float3 midPoint = (ourVertex[index1].Position + ourVertex[index2].Position + ourVertex[index3].Position) / 3.0f;

			// Obtener la normal de la cara
			float3 faceNormal = (ourVertex[index2].Position - ourVertex[index1].Position).Cross(ourVertex[index3].Position - ourVertex[index1].Position);

			// Normalizar la normal de la cara
			faceNormal = faceNormal.Normalized();

			// Punto inicial de la línea en el punto medio de la cara
			glVertex3f(midPoint.x, midPoint.y, midPoint.z);

			// Punto final de la línea desplazado según la normal de la cara
			float scale = 0.1f;  // Factor de escala para las líneas de las normales
			glVertex3f(midPoint.x + scale * faceNormal.x, midPoint.y + scale * faceNormal.y, midPoint.z + scale * faceNormal.z);
		}
		glEnd();

	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_COORD_ARRAY);

}

void MeshData::InitBuffers() {
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->ourVertex.size(), &this->ourVertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), &this->indices[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}