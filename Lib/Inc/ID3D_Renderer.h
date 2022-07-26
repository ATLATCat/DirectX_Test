#pragma once

#ifdef ATLATDXRENDERER_EXPORTS
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif // ATLATDXRENDERER_EXPORTS

namespace ATLAT
{
class RENDERER_API ID3D_Renderer abstract
{
public:
	virtual ~ID3D_Renderer() {};

	virtual void Init(const RendererInitData& initData) abstract;
	virtual void Render() abstract;
	virtual void RenderBegin() abstract;
	virtual void RenderEnd() abstract;

	virtual void ResizeWindow(int32 width, int32 height) abstract;
	virtual void OnResizeWindow() abstract;

#pragma region DataSet
	virtual void CreateMesh(UID* out_uid) abstract;
	virtual void SetVertices(const void* buffer, uint32 vertexSize, uint32 count, UID uid) abstract;
	virtual void SetIndices(const void* buffer, uint32 indexSize, uint32 count, UID uid) abstract;

	virtual void CreateMateiral(UID* out_uid, uint32 size) abstract;
	virtual void SetMaterialData(const void* buffer, uint32 size, UID uid_material, uint32 offset = 0) abstract;

	virtual void LoadTexture(const std::wstring& path, UID* out_uid) abstract;
	virtual void SetTexture(UID uid_material, UID uid_texture) abstract;
	
	virtual void LoadShader(const std::wstring& path, UID* out_uid) abstract;
	virtual void SetShader(UID uid_material, UID uid_texture) abstract;
#pragma endregion

#pragma region Draw
	virtual void DrawMesh(UID uid_mesh, UID uid_material, uint8 layer) abstract;
#pragma endregion

	virtual void SetConstantBuffer(CONSTANT_BUFFER_TYPE type, const void* data, uint32 size, uint8 layer) abstract;
	
	static std::shared_ptr<ID3D_Renderer> GetRenderer();
	static void Release();
protected:
	static std::shared_ptr<ID3D_Renderer> _instance;
};
}

typedef std::shared_ptr<ATLAT::ID3D_Renderer>(*GetRenderFunction)();
typedef void(*GetReleaseFunction)();

extern "C" RENDERER_API GetRenderFunction GetRenderer();
extern "C" RENDERER_API GetReleaseFunction GetRelease();

