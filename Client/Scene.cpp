#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraController.h"
#include "Engine.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Light.h"
#include "Material.h"
#include "Sprite.h"

void Scene::Load(const std::wstring& path)
{
#pragma region »óÀÚ µ¥ÀÌÅÍ ÃÊ±âÈ­ 
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;
	std::vector<Vertex> vec(24);

	// ¾Õ¸é
	vec[0] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vec[1] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vec[2] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vec[3] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	// µÞ¸é
	vec[4] = Vertex{ Vector3(-w2, -h2, +d2), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	vec[5] = Vertex{ Vector3(+w2, -h2, +d2), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	vec[6] = Vertex{ Vector3(+w2, +h2, +d2), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	vec[7] = Vertex{ Vector3(-w2, +h2, +d2), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	// À­¸é
	vec[8] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vec[9] = Vertex{ Vector3(-w2, +h2, +d2), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vec[10] = Vertex{ Vector3(+w2, +h2, +d2), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vec[11] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f) };
	// ¾Æ·§¸é
	vec[12] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	vec[13] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	vec[14] = Vertex{ Vector3(+w2, -h2, +d2), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	vec[15] = Vertex{ Vector3(-w2, -h2, +d2), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f) };
	// ¿ÞÂÊ¸é
	vec[16] = Vertex{ Vector3(-w2, -h2, +d2), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f) };
	vec[17] = Vertex{ Vector3(-w2, +h2, +d2), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f) };
	vec[18] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f) };
	vec[19] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f) };
	// ¿À¸¥ÂÊ¸é
	vec[20] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) };
	vec[21] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) };
	vec[22] = Vertex{ Vector3(+w2, +h2, +d2), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) };
	vec[23] = Vertex{ Vector3(+w2, -h2, +d2), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) };

	std::vector<uint32> idx(36);

	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// µÞ¸é
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// À­¸é
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// ¾Æ·§¸é
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ¿ÞÂÊ¸é
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ¿À¸¥ÂÊ¸é
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();

	std::shared_ptr<Transform> transform = std::make_shared<Transform>();

	std::shared_ptr<Mesh> mesh = resourceManager.CreateMesh();
	mesh->SetVertices(vec);
	mesh->SetIndeices(idx);

	std::shared_ptr<Texture> texture = resourceManager.LoadTexture(L"Textures\\WoodCrate02.dds");
	std::shared_ptr<Shader> shader = resourceManager.LoadShader(L"Shader\\Deferred.hlsli");

	std::shared_ptr<Material> material = resourceManager.CreateMaterial();
	material->SetShader(shader);
	material->SetTexture(texture);
	MaterialData data;
	data.ambient = { 0.5f, 0.5f, 0.5f, 1.f };
	data.diffuse = { 0.5f, 0.5f, 0.5f, 1.f };
	data.specular = { 0.5f, 0.5f, 0.5f, 1.f };
	data.reflect = { 0, 0, 0, 0 };
	material->SetData(data);

	std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
	meshRenderer->SetMesh(mesh);
	meshRenderer->SetMaterial(material);

	obj->AddComponent(meshRenderer);
	obj->AddComponent(transform);

	_gameObjects.push_back(obj);

#pragma endregion

#pragma region Sprite
	//texture = resourceManager.LoadTexture(L"Textures\\WoodCrate02.dds");
	//shader = resourceManager.LoadShader(L"Shader\\DefaultSprite.hlsli");

	//material = resourceManager.CreateMaterial();
	//material->SetShader(shader);
	//material->SetTexture(texture);

	//data.ambient = { 1.f, 1.f, 1.f, 1.f };
	//data.diffuse = { 1.f, 1.f, 1.f, 1.f };
	//data.specular = { 0.f, 0.f, 0.f, 1.f };
	//data.reflect = { 0, 0, 0, 0 };
	//material->SetData(data);

	//std::shared_ptr<Sprite> sprite1 = std::make_shared<Sprite>();
	//sprite1->SetMaterial(material);

	//transform = std::make_shared<Transform>();
	//transform->TransformVector({ 0.f, 0, 10 });

	//obj = std::make_shared<GameObject>();
	//obj->AddComponent(sprite1);
	//obj->AddComponent(transform);
	//obj->GetLayer().SetValue(LayerType::UI);

	//_gameObjects.push_back(obj);
#pragma endregion

	std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>();
	cameraObj = std::make_shared<GameObject>();
	transform = std::make_shared<Transform>();
	transform->TransformVector({ 0, 0, -10 });
	cameraObj->AddComponent(transform);

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	auto& windowInfo = Engine::GetWindowInfo();
	camera->SetProjectionMatrix(DirectX::XM_PI * 0.25f, static_cast<float>(windowInfo.width) / windowInfo.height, 1.f, 1000.f);
	cameraObj->AddComponent(camera);

	std::shared_ptr<CameraController> _controller = std::make_shared<CameraController>();
	cameraObj->AddComponent(_controller);

	std::shared_ptr<MeshRenderer> gridMeshRenderer = std::make_shared<MeshRenderer>();

	//std::shared_ptr<Mesh> gridMesh = std::make_shared<Mesh>();
	//mesh->SetVertices();
	//mesh->SetIndeices();
	//gridMesh->SetMesh()

	_gameObjects.push_back(cameraObj);

	std::shared_ptr<Light> light = std::make_shared<Light>();
	std::shared_ptr<GameObject> lightObj = std::make_shared<GameObject>();
	lightObj->AddComponent(light);
	_gameObjects.push_back(lightObj);
}
