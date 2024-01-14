#include "MeshImporter.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include <vector>
#include "CompMesh.h"
#include "CompTransform.h"
#include "CompCamera.h"
#include "Application.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleRenderer3D.h"
#include "Globals.h"
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")
#pragma comment (lib, "Assimp/libx86/assimp.lib")


void Importer::DeleteMesh(MeshData* mesh2delete)
{
	for (int i = 0; i < App->renderer3D->ourMeshes.size(); i++) {

		if (mesh2delete == App->renderer3D->ourMeshes[i]) {
			App->renderer3D->ourMeshes.erase(App->renderer3D->ourMeshes.begin() + i);
		}

	}
}

GameObject* Importer::LoadMesh(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		std::string filePathStr(file_path);
		size_t lastSlash = filePathStr.find_last_of("/\\");
		std::string fileName = (lastSlash != std::string::npos) ? filePathStr.substr(lastSlash + 1) : filePathStr;
		size_t lastDot = fileName.find_last_of(".");
		if (lastDot != std::string::npos)
		{
			fileName = fileName.substr(0, lastDot);
		}

		GameObject* _go = new GameObject(fileName);

		App->renderer3D->GOtotal++;
		ProcessNode(scene, scene->mRootNode, _go, file_path);
		aiReleaseImport(scene);

		return _go;
	}


}

void Importer::ProcessNode(const aiScene* scene, aiNode* node, GameObject* GO, const char* file_path)
{
	if (node->mNumMeshes == 0 && node->mNumChildren == 0) return;

	if (node->mNumMeshes != 0) {

		for (int i = 0; i < node->mNumMeshes; i++) {

			MeshData* temp = new MeshData();
			aiMesh* AiMesh = scene->mMeshes[node->mMeshes[i]];
			for (unsigned int o = 0; o < AiMesh->mNumVertices; o++)
			{
				Vertex tempvertex;
				float3 vector;
				vector.x = AiMesh->mVertices[o].x;
				vector.y = AiMesh->mVertices[o].y;
				vector.z = AiMesh->mVertices[o].z;
				tempvertex.Position = vector;

				LOG("New mesh with %d vertices", AiMesh->mNumVertices);

				if (AiMesh->HasNormals())
				{
					tempvertex.Normal.x = AiMesh->mNormals[o].x;
					tempvertex.Normal.y = AiMesh->mNormals[o].y;
					tempvertex.Normal.z = AiMesh->mNormals[o].z;

				}
				if (AiMesh->HasTextureCoords(0))
				{
					tempvertex.TexCoords.x = AiMesh->mTextureCoords[0][o].x;
					tempvertex.TexCoords.y = AiMesh->mTextureCoords[0][o].y;

				}
				else
				{
					tempvertex.TexCoords.x = 0.0f;
					tempvertex.TexCoords.y = 0.0f;
				}

				temp->ourVertex.push_back(tempvertex);
			}

			if (AiMesh->HasFaces())
			{

				temp->indices.resize(AiMesh->mNumFaces * 3);// assume each face is a triangle

				for (uint y = 0; y < AiMesh->mNumFaces; y++)
				{
					if (AiMesh->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {

						memcpy(&temp->indices[y * 3], AiMesh->mFaces[y].mIndices, 3 * sizeof(unsigned int));

					}
				}
			}
			temp->textid = nullptr;

			App->renderer3D->ourMeshes.push_back(temp);
			App->renderer3D->ourMeshes.back()->InitBuffers();

			GameObject* go;
			
			go = new GameObject(node->mName.C_Str());
			go->GetComponent<CompMesh>()->SetMesh(App->renderer3D->ourMeshes.back());
			go->GetComponent<CompMesh>()->name = node->mName.C_Str();
			go->GetComponent<CompMesh>()->path = file_path;
			go->GetComponent<CompMesh>()->_ourMeshes = App->renderer3D->ourMeshes;


			aiMatrix4x4 TransformMat = node->mTransformation;

			aiVector3D scale, position, rotation;
			aiQuaternion QuatRotation;

			TransformMat.Decompose(scale, QuatRotation, position);
			rotation = QuatRotation.GetEuler();
			go->GetComponent<CompTransform>()->position = float3(position.x, position.y, position.z);
			go->GetComponent<CompTransform>()->rotation = Quat(QuatRotation.x, QuatRotation.y, QuatRotation.z, QuatRotation.w);

			GO->AddChild(go);

			TextureImporter::ImportTexture(GetPathFromScene(scene, node->mMeshes[i], file_path), go);
		}

		if (GO->parent == nullptr) {
			App->scene->root->AddChild(GO);
		}


	}

	for (int i = 0; i < node->mNumChildren; i++) {

		ProcessNode(scene, node->mChildren[i], GO, file_path);
	
	}


}


void MeshData::Draw(GLuint checkers, float4x4 matrix) {
	
	glPushMatrix();
	glMultMatrixf(matrix.ptr());
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
	if(printcheckers){
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
			unsigned int index1 = indices[i];
			unsigned int index2 = indices[i + 1];
			unsigned int index3 = indices[i + 2];

			float3 midPoint = (ourVertex[index1].Position + ourVertex[index2].Position + ourVertex[index3].Position) / 3.0f;

			float3 faceNormal = (ourVertex[index2].Position - ourVertex[index1].Position).Cross(ourVertex[index3].Position - ourVertex[index1].Position);

			faceNormal = faceNormal.Normalized();

			glVertex3f(midPoint.x, midPoint.y, midPoint.z);

			float scale = 0.1f; 
			glVertex3f(midPoint.x + scale * faceNormal.x, midPoint.y + scale * faceNormal.y, midPoint.z + scale * faceNormal.z);
		}
		glEnd();

	}
	if (printAABB) {
	
		float3 corners1[8];
		localAABB.GetCornerPoints(corners1);
		glLineWidth(5.0f);
		App->renderer3D->DrawBox(corners1, { 0.0f, 0.0f, 1.0f });


		
	}
	printAABB = false;
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

void MeshData::InitBuffers() {

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * ourVertex.size(), &ourVertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


AABB MeshData::GenLocalAABB()
{
	localAABB.SetNegativeInfinity();
	localAABB.Enclose(&ourVertex.at(0).Position, ourVertex.size());
	
	return localAABB;
}

AABB MeshData::GenGlobalBB(GameObject* go)
{

	oBB = GenLocalAABB();
	oBB.Transform(go->GetComponent<CompTransform>()->GetGlobalMatrix());

	aABB.SetNegativeInfinity();
	aABB.Enclose(oBB);

	return aABB;
}

std::string Importer::GetPathFromScene(const aiScene* scene, int index, std::string path)
{

	if (scene->HasMaterials())
	{
		aiMaterial* MaterialIndex = scene->mMaterials[scene->mMeshes[index]->mMaterialIndex];
		if (MaterialIndex->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

			aiString TextPath;
			MaterialIndex->GetTexture(aiTextureType_DIFFUSE, 0, &TextPath);

			for (int i = 0; i < path.size(); i++)
			{
				if (path[i] == '\\')
				{
					path[i] = '/';
				}
			}

			std::string NormTextPath = TextPath.C_Str();

			for (int i = 0; i < NormTextPath.size(); i++)
			{
				if (NormTextPath[i] == '\\')
				{
					NormTextPath[i] = '/';
				}
			}

			std::string AssetsPath = path;
			uint AssetsPos = AssetsPath.find("Assets/");

			if (AssetsPos < AssetsPath.size())
			AssetsPath = AssetsPath.substr(AssetsPos, AssetsPath.find_last_of("/") - AssetsPos);

			if (AssetsPos < AssetsPath.size()) 
			AssetsPath = AssetsPath.substr(AssetsPos, AssetsPath.find_last_of("/") - AssetsPos);

			AssetsPath.append("/Textures/").append(TextPath.C_Str());

			return AssetsPath;
		}
	}

	return "";

}

