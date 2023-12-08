#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#define UID Uint64

#include "Module.h"
#include <string>

class Application;

//class Resource
//{
//public:
//	enum Type {
//		texture,
//		mesh,
//		audio,
//		scene,
//		bone,
//		animation,
//		unknown
//	};
//	Resource(UID uid, Type type);
//	virtual ~Resource();
//	Resource::Type GetType() const;
//	UID GetUID() const;
//	const char* GetAssetFile() const;
//	const char* GetLibraryFile() const;
//	bool IsLoadedToMemory() const;
//	bool LoadToMemory();
//	uint GetReferenceCount() const;
//	virtual void Save(Config& config) const;
//	virtual void Load(const Config& config);
//	virtual bool LoadInMemory() = 0;
//protected:
//	UID uid = 0;
//	std::string assetsFile;
//	std::string Libraryile;
//	Type type = unknown;
//	uint referenceCount = 0;
//};
//
//class ResourceTexture : public Resource
//{
//public:
//	enum Format {
//		color_index,
//		rgb,
//		rgba,
//		bgr,
//		bgra,
//		luminance,
//		unknownFormat
//	};
//	ResourceTexture(UID id);
//	virtual ~ResourceTexture();
//	bool LoadInMemory() override;
//	void Save(Config& config) const override;
//	void Load(const Config& config) override;
//public:
//	uint width = 0;
//	uint height = 0;
//	uint depth = 0;
//	uint mips = 0;
//	uint bytes = 0;
//	uint gpu_id = 0;
//	Format format = unknownFormat;
//};
class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();
	bool Start();
	//UID Find(const char* file_in_assets) const;
	//UID ImportFile(const char* new_file_in_assets);
	//UID GenerateNewUID();
	//const Resource* RequestResource(UID uid) const;
	//Resource* RequestResource(UID uid);
	//void ReleaseResource(UID uid);
private:
	std::string name;
	//Resource* CreateNewResource(const char* assetsFile, Resource::Type type);
	//std::map<UID, Resource*> resources;
};


#endif // !__MODULE_RESOURCES_H_