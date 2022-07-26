#include "pch.h"
#include "Parser.h"
#include <sstream>
#include <filesystem>
#include <fstream>

using namespace std;

void ASEParser::Parser::LoadFile(const char* filename)
{
	ifstream fin = ifstream(filename);

	if (fin.is_open())
	{
	}
	else
	{
		MessageBox(0, L"Parser file load Fail", 0, 0);
		return;
	}
}

void ASEParser::Parser::Parse(wstring filePath)
{
	Clear();

	ifstream fin = ifstream(filePath);
	if (!fin.is_open())
	{
		MessageBox(0, L"Parser file load Fail", 0, 0);
		return;
	}
	fin.seekg(0, ios::end);
	string buffer(fin.tellg(), '\0');
	fin.seekg(0, ios::beg);
	fin.read(&buffer[0], buffer.size());

	stringstream ss(buffer);

	string tokenString;
	string ignore;
	ASEToken token = TOKENR_END;
	Lexer lexer;

	_meshIndex = -1;

	while (ss)
	{
		ss >> tokenString;
		token = lexer.GetToken(tokenString);
		if(token == ASEToken::TOKENR_END)
			continue;
		switch (token)
		{
		case ASEParser::TOKENR_3DSMAX_ASCIIEXPORT:
			break;
		case ASEParser::TOKENR_COMMENT:
			break;
		case ASEParser::TOKENR_SCENE:
			break;
		case ASEParser::TOKENR_MATERIAL_LIST:
			break;
		case ASEParser::TOKENR_MATERIAL:
		{
			ss >> _materialIndex;
		}
			break;
		case ASEParser::TOKENR_LIGHTOBJECT:
			break;
		case ASEParser::TOKENR_GEOMOBJECT:
		{
			if (_meshIndex != -1)
			{
				_meshes[_meshIndex]->vertices = _vertices;
				_meshes[_meshIndex]->indices = _indices;
			}
			ObjectClear();

			shared_ptr<GameObject> gameObject = make_shared<GameObject>();
			_gameObjects.push_back(gameObject);
			shared_ptr<Mesh> mesh = make_shared<Mesh>();
			_meshes.push_back(mesh);
			gameObject->mesh = mesh;
			_meshIndex = static_cast<int>(_meshes.size() - 1);
		}
			break;
		case ASEParser::TOKENR_NODE_NAME:
		{
			char ignore;
			ss >> ignore;
			ss >> _nodeName;
			_nodeName.erase(_nodeName.size() - 1, 1);
			_gameObjects[_gameObjects.size() - 1]->name = _nodeName;
		}
			break;
		case ASEParser::TOKENR_NODE_TM:
			break;
		case ASEParser::TOKENR_MESH:
		{
		}
			break;
		case ASEParser::TOKENR_MESH_VERTEX:
		{
			int index;
			ss >> index;
			ss >> _verticesOrigin[index].position.x;
			ss >> _verticesOrigin[index].position.z;
			ss >> _verticesOrigin[index].position.y;
			Vector4 vec = XMLoadFloat3(&_verticesOrigin[index].position);
			
			Matrix inverseWorld = XMMatrixInverse(nullptr, XMLoadFloat4x4(&_nodeMatrix));
			vec = XMVector3Transform(vec, inverseWorld);
			XMStoreFloat3(&_verticesOrigin[index].position, vec);
		}
			break;
		case ASEParser::TOKENR_MESH_FACE_LIST:
			break;
		case ASEParser::TOKENR_MESH_FACE:
		{
			int index;
			ss >> index;
			ss >> ignore >> ignore;
			ss >> _indicesOrigin[static_cast<uint64_t>(index * 3)];
			ss >> ignore;
			ss >> _indicesOrigin[static_cast<uint64_t>(index * 3 + 1)];
			ss >> ignore;
			ss >> _indicesOrigin[static_cast<uint64_t>(index * 3 + 2)];
		}
			break;
		case ASEParser::TOKENR_MESH_NUMTVERTEX:
		{
			int size;
			ss >> size;
			_textureUV.reserve(size);
		}
			break;
		case ASEParser::TOKENR_MESH_NUMCVERTEX:
			break;
		case ASEParser::TOKENR_MESH_NORMALS:
		{
		}
			break;
		case ASEParser::TOKENR_MESH_FACENORMAL:
		{
			ss >> _faceNormalIndex;
			_faceNormalCount = 0;
		}
			break;
		case ASEParser::TOKENR_MESH_VERTEXNORMAL:
		{
			int vertexIndex;
			ss >> vertexIndex;
			Vector3 normal;
			ss >> normal.x;
			ss >> normal.z;
			ss >> normal.y;
			Vertex vertex = _verticesOrigin[vertexIndex];
			vertex.normal = normal;
			if (_textureUV.size() > 0)
			{
				vertex.uv.x = _textureUV[_textureIndices[static_cast<uint64_t>(_faceNormalIndex * 3 + _faceNormalCount)]].x;
				vertex.uv.y = _textureUV[_textureIndices[static_cast<uint64_t>(_faceNormalIndex * 3 + _faceNormalCount)]].y;
			}
			_indices.push_back(static_cast<UINT>(_vertices.size()));
			_vertices.push_back(vertex);
			_faceNormalCount++;
		}
			break;
		case ASEParser::TOKENR_HELPEROBJECT: 
		{
			shared_ptr<GameObject> gameObject = make_shared<GameObject>();
			_gameObjects.push_back(gameObject);
		}
			break;
		case ASEParser::TOKENR_NODE_PARENT:
		{
			char ignore;
			ss >> ignore;
			ss >> _nodeParent;
			_nodeParent.erase(_nodeParent.size() - 1, 1);
			bool isFind = false;

			for (auto obj : _gameObjects)
			{
				if (obj->name == _nodeParent)
				{
					isFind = true;
					auto child = _gameObjects[_gameObjects.size() - 1];
					obj->children.push_back(child);
					child->parent = obj;
					break;
				}
			}
			if (isFind == false)
			{
				MessageBox(0, L"³¢¿ä¿À¿Ë ºÎ¸ð°¡ ¾øÂ¥³ª!", 0, 0);
			}
		}
		break;
		case ASEParser::TOKENR_HELPER_CLASS:
			break;
		case ASEParser::TOKENR_INHERIT_POS:
			break;
		case ASEParser::TOKENR_INHERIT_ROT:
			break;
		case ASEParser::TOKENR_INHERIT_SCL:
			break;
		case ASEParser::TOKENR_TM_ROW0:
		{
			ss >> _nodeMatrix._11;
			ss >> _nodeMatrix._13;
			ss >> _nodeMatrix._12;
			_nodeMatrix._14 = 0;
		}
		break;
		case ASEParser::TOKENR_TM_ROW1:
		{
			ss >> _nodeMatrix._31;
			ss >> _nodeMatrix._33;
			ss >> _nodeMatrix._32;
			_nodeMatrix._34 = 0;
		}
		break;
		case ASEParser::TOKENR_TM_ROW2:
		{
			ss >> _nodeMatrix._21;
			ss >> _nodeMatrix._23;
			ss >> _nodeMatrix._22;
			_nodeMatrix._24 = 0;
		}
		break;
		case ASEParser::TOKENR_TM_ROW3:
		{
			ss >> _nodeMatrix._41;
			ss >> _nodeMatrix._43;
			ss >> _nodeMatrix._42;
			_nodeMatrix._44 = 1;
			Matrix world = XMLoadFloat4x4(&_nodeMatrix);
			_gameObjects[_gameObjects.size() - 1]->matrix = world;
		}
		break;
		case ASEParser::TOKENR_TM_POS:
			break;
		case ASEParser::TOKENR_TM_ROTAXIS:
			break;
		case ASEParser::TOKENR_TM_ROTANGLE:
			break;
		case ASEParser::TOKENR_TM_SCALE:
			break;
		case ASEParser::TOKENR_TM_SCALEAXIS:
			break;
		case ASEParser::TOKENR_TM_SCALEAXISANG:
			break;
		case ASEParser::TOKENR_BOUNDINGBOX_MIN:
			break;
		case ASEParser::TOKENR_BOUNDINGBOX_MAX:
			break;
		case ASEParser::TOKENR_TIMEVALUE:
			break;
		case ASEParser::TOKENR_MESH_NUMVERTEX:
		{
			int size;
			ss >> size;
			_verticesOrigin.reserve(size);
			for (int i = 0; i < size; ++i)
			{
				_verticesOrigin.push_back(Vertex());
			}
		}
		break;
		case ASEParser::TOKENR_MESH_NUMFACES:
		{
			int size;
			ss >> size;
			size *= 3;
			_indicesOrigin.reserve(size);
			for (int i = 0; i < size; ++i)
			{
				_indicesOrigin.push_back(0);
			}
		}
		break;
		case ASEParser::TOKENR_GROUP:
			break;
		case ASEParser::TOKENR_MESH_TVERT:
		{
			int index;
			ss >> index;
			Vector2 uv;
			ss >> uv.x;
			ss >> uv.y;
			uv.y = 1 - uv.y;
			_textureUV.push_back(uv);
		}
		break;
		case ASEParser::TOKENR_MESH_TFACELIST:
		{
		}
		break;
		case ASEParser::TOKENR_MESH_TVERTLIST:
			break;
		case ASEParser::TOKENR_PROP_MOTIONBLUR:
			break;
		case ASEParser::TOKENR_PROP_CASTSHADOW:
			break;
		case ASEParser::TOKENR_PROP_RECVSHADOW:
			break;
		case ASEParser::TOKENR_MATERIAL_REF:
			break;
		case ASEParser::TOKENR_SCENE_FILENAME:
			break;
		case ASEParser::TOKENR_SCENE_FIRSTFRAME:
			break;
		case ASEParser::TOKENR_SCENE_LASTFRAME:
			break;
		case ASEParser::TOKENR_SCENE_FRAMESPEED:
			break;
		case ASEParser::TOKENR_SCENE_TICKSPERFRAME:
			break;
		case ASEParser::TOKENR_SCENE_ENVMAP:
			break;
		case ASEParser::TOKENR_MAP_NAME:
			break;
		case ASEParser::TOKENR_MAP_CLASS:
			break;
		case ASEParser::TOKENR_MAP_SUBNO:
			break;
		case ASEParser::TOKENR_MAP_AMOUNT:
			break;
		case ASEParser::TOKENR_SCENE_AMBIENT_STATIC:
			break;
		case ASEParser::TOKENR_MATERIAL_COUNT:
		{
			int count;
			ss >> count;
			_materials.reserve(count);
			for (int i = 0; i < count; ++i)
			{
				_materials.push_back(std::make_shared<Material>());
				_materials[i]->shaderName = L"DefaultShader.cso";
			}
		}
		break;
		case ASEParser::TOKENR_MATERIAL_NAME:
			break;
		case ASEParser::TOKENR_MATERIAL_CLASS:
			break;
		case ASEParser::TOKENR_MATERIAL_AMBIENT:
		{
			std::shared_ptr<Material> material = _materials[_materialIndex];
			ss >> material->ambient.x;
			ss >> material->ambient.y;
			ss >> material->ambient.z;
			material->ambient.w = 1.f;
		}
		break;
		case ASEParser::TOKENR_MATERIAL_DIFFUSE:
		{
			std::shared_ptr<Material> material = _materials[_materialIndex];
			ss >> material->diffuse.x;
			ss >> material->diffuse.z;
			ss >> material->diffuse.y;
			material->diffuse.w = 1.f;
		}
		break;
		case ASEParser::TOKENR_MATERIAL_SPECULAR:
		{
			std::shared_ptr<Material> material = _materials[_materialIndex];
			ss >> material->specular.x;
			ss >> material->specular.z;
			ss >> material->specular.y;
			material->specular.w = 1;
		}
		break;
		case ASEParser::TOKENR_MATERIAL_SHINE:
		{
		}
		break;
		case ASEParser::TOKENR_MATERIAL_SHINESTRENGTH:
			break;
		case ASEParser::TOKENR_MATERIAL_TRANSPARENCY:
			break;
		case ASEParser::TOKENR_MATERIAL_WIRESIZE:
			break;
		case ASEParser::TOKENR_MATERIAL_SHADING:
			break;
		case ASEParser::TOKENR_MATERIAL_XP_FALLOFF:
			break;
		case ASEParser::TOKENR_MATERIAL_SELFILLUM:
			break;
		case ASEParser::TOKENR_MATERIAL_TWOSIDED:
			break;
		case ASEParser::TOKENR_MATERIAL_FALLOFF:
			break;
		case ASEParser::TOKENR_MATERIAL_XP_TYPE:
			break;
		case ASEParser::TOKENR_MAP_DIFFUSE:
			break;
		case ASEParser::TOKENR_MESH_VERTEX_LIST:
		{
		}
		break;
		case ASEParser::TOKENR_MESH_NUMTVFACES:
		{
			int size;
			ss >> size;
			_textureIndices.reserve(size);
		}
		break;
		case ASEParser::TOKENR_MESH_CVERTLIST:
			break;
		case ASEParser::TOKENR_LIGHT_TYPE:
			break;
		case ASEParser::TOKENR_LIGHT_SHADOWS:
			break;
		case ASEParser::TOKENR_LIGHT_USELIGHT:
			break;
		case ASEParser::TOKENR_LIGHT_SPOTSHAPE:
			break;
		case ASEParser::TOKENR_LIGHT_USEGLOBAL:
			break;
		case ASEParser::TOKENR_LIGHT_ABSMAPBIAS:
			break;
		case ASEParser::TOKENR_LIGHT_OVERSHOOT:
			break;
		case ASEParser::TOKENR_LIGHT_SETTINGS:
			break;
		case ASEParser::TOKENR_LIGHT_COLOR:
			break;
		case ASEParser::TOKENR_LIGHT_INTENS:
			break;
		case ASEParser::TOKENR_LIGHT_ASPECT:
			break;
		case ASEParser::TOKENR_LIGHT_TDIST:
			break;
		case ASEParser::TOKENR_LIGHT_MAPBIAS:
			break;
		case ASEParser::TOKENR_LIGHT_MAPRANGE:
			break;
		case ASEParser::TOKENR_LIGHT_MAPSIZE:
			break;
		case ASEParser::TOKENR_LIGHT_RAYBIAS:
			break;
		case ASEParser::TOKENR_MESH_SMOOTHING:
			break;
		case ASEParser::TOKENR_MESH_MTLID:
			break;
		case ASEParser::TOKENR_WIREFRAME_COLOR:
			break;
		case ASEParser::TOKENR_TM_ANIMATION:
			break;
		case ASEParser::TOKENR_CONTROL_POS_TRACK:
			break;
		case ASEParser::TOKENR_CONTROL_ROT_TRACK:
			break;
		case ASEParser::TOKENR_CONTROL_POS_SAMPLE:
			break;
		case ASEParser::TOKENR_CONTROL_ROT_SAMPLE:
			break;
		case ASEParser::TOKENR_CAMERAOBJECT:
			break;
		case ASEParser::TOKENR_CAMERA_TYPE:
			break;
		case ASEParser::TOKENR_CAMERA_SETTINGS:
			break;
		case ASEParser::TOKENR_CAMERA_NEAR:
			break;
		case ASEParser::TOKENR_CAMERA_FAR:
			break;
		case ASEParser::TOKENR_CAMERA_FOV:
			break;
		case ASEParser::TOKENR_CAMERA_TDIST:
			break;
		case ASEParser::TOKENR_NUMSUBMTLS:
			break;
		case ASEParser::TOKENR_SUBMATERIAL:
			break;
		case ASEParser::TOKENR_MAP_SPECULAR:
			break;
		case ASEParser::TOKENR_MAP_SHINE:
			break;
		case ASEParser::TOKENR_MAP_GENERIC:
			break;
		case ASEParser::TOKENR_BITMAP:
		{
			string filePath;
			ss >> filePath;
			filePath.erase(0, 1);
			filePath.erase(filePath.size() - 1, 1);
			_materials[_materials.size() - 1]->textureName.assign(filePath.begin(), filePath.end());
		}
		break;
		case ASEParser::TOKENR_MAP_TYPE:
			break;
		case ASEParser::TOKENR_UVW_U_OFFSET:
			break;
		case ASEParser::TOKENR_UVW_V_OFFSET:
			break;
		case ASEParser::TOKENR_UVW_U_TILING:
			break;
		case ASEParser::TOKENR_UVW_V_TILING:
			break;
		case ASEParser::TOKENR_UVW_ANGLE:
			break;
		case ASEParser::TOKENR_UVW_BLUR:
			break;
		case ASEParser::TOKENR_UVW_BLUR_OFFSET:
			break;
		case ASEParser::TOKENR_UVW_NOUSE_AMT:
			break;
		case ASEParser::TOKENR_UVW_NOISE_SIZE:
			break;
		case ASEParser::TOKENR_UVW_NOISE_LEVEL:
			break;
		case ASEParser::TOKENR_UVW_NOISE_PHASE:
			break;
		case ASEParser::TOKENR_BITMAP_FILTER:
			break;
		case ASEParser::TOKENR_MESH_MAPPINGCHANNEL:
			break;
		case ASEParser::TOKENR_MESH_TFACE:
		{
			int faceIndex;
			ss >> faceIndex;
			int index;
			ss >> index;
			_textureIndices.push_back(index);
			ss >> index;
			_textureIndices.push_back(index);
			ss >> index;
			_textureIndices.push_back(index);
		}
		break;
		case ASEParser::TOKENR_CONTROL_POS_BEZIER:
			break;
		case ASEParser::TOKENR_CONTROL_BEZIER_POS_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_ROT_TCB:
			break;
		case ASEParser::TOKENR_CONTROL_TCB_ROT_KEY:
			break;
		case ASEParser::TOKENR_MAP_OPACITY:
			break;
		case ASEParser::TOKENR_MATERIAL_FACEMAP:
			break;
		case ASEParser::TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case ASEParser::TOKENR_MESH_NUMBONE:
			break;
		case ASEParser::TOKENR_BONE_LIST:
			break;
		case ASEParser::TOKENR_BONE_NAME:
			break;
		case ASEParser::TOKENR_MESH_WVERTEXS:
			break;
		case ASEParser::TOKENR_MESH_WEIGHT:
			break;
		case ASEParser::TOKENR_BONE_BLENGING_WEIGHT:
			break;
		case ASEParser::TOKENR_MAP_REFLECT:
			break;
		case ASEParser::TOKENR_MAP_REFRACT:
			break;
		case ASEParser::TOKENR_MAP_BUMP:
			break;
		case ASEParser::TOKENR_SCENE_BACKGROUND_STATIC:
			break;
		case ASEParser::TOKENR_MAP_SELFILLUM:
			break;
		case ASEParser::TOKENR_LIGHT_EXCLUDELIST:
			break;
		case ASEParser::TOKENR_LIGHT_NUMEXCLUDED:
			break;
		case ASEParser::TOKENR_LIGHT_EXCLUDED_INCLUDE:
			break;
		case ASEParser::TOKENR_LIGHT_EXCLUDED_AFFECT_ILLUM:
			break;
		case ASEParser::TOKENR_LIGHT_EXCLUDED_AFFECT_SHADOW:
			break;
		case ASEParser::TOKENR_LIGHT_EXCLUDED:
			break;
		case ASEParser::TOKENR_LIGHT_HOTSPOT:
			break;
		case ASEParser::TOKENR_LIGHT_FALLOFF:
			break;
		case ASEParser::TOKENR_MESH_FACEMAPLIST:
			break;
		case ASEParser::TOKENR_MESH_FACEMAP:
			break;
		case ASEParser::TOKENR_MESH_FACEMAPVERT:
			break;
		case ASEParser::TOKENR_BITMAP_INVERT:
			break;
		case ASEParser::TOKENR_SHAPEOBJECT:
		{
			shared_ptr<GameObject> gameObject = make_shared<GameObject>();
			_gameObjects.push_back(gameObject);
		}
		break;
		case ASEParser::TOKENR_SHAPE_LINECOUNT:
			break;
		case ASEParser::TOKENR_SHAPE_LINE:
			break;
		case ASEParser::TOKENR_SHAPE_VERTEXCOUNT:
			break;
		case ASEParser::TOKENR_SHAPE_VERTEX_KNOT:
			break;
		case ASEParser::TOKENR_SHAPE_VERTEX_INTERP:
			break;
		case ASEParser::TOKENR_SHAPE_CLOSED:
			break;
		case ASEParser::TOKENR_IK_JOINT:
			break;
		case ASEParser::TOKENR_IK_TYPE:
			break;
		case ASEParser::TOKENR_IK_DOF:
			break;
		case ASEParser::TOKENR_IK_XACTIVE:
			break;
		case ASEParser::TOKENR_IK_YACTIVE:
			break;
		case ASEParser::TOKENR_IK_ZACTIVE:
			break;
		case ASEParser::TOKENR_IK_XLIMITED:
			break;
		case ASEParser::TOKENR_IK_YLIMITED:
			break;
		case ASEParser::TOKENR_IK_ZLIMITED:
			break;
		case ASEParser::TOKENR_IK_XEASE:
			break;
		case ASEParser::TOKENR_IK_YEASE:
			break;
		case ASEParser::TOKENR_IK_ZEASE:
			break;
		case ASEParser::TOKENR_IK_LIMITEXACT:
			break;
		case ASEParser::TOKENR_IK_JOINTINFO:
			break;
		case ASEParser::TOKENR_LIGHT_ATTNSTART:
			break;
		case ASEParser::TOKENR_LIGHT_ATTNEND:
			break;
		case ASEParser::TOKENR_MAP_AMBIENT:
			break;
		case ASEParser::TOKENR_MESH_VERTCOL:
			break;
		case ASEParser::TOKENR_MESH_NUMCVFACES:
			break;
		case ASEParser::TOKENR_MESH_CFACELIST:
			break;
		case ASEParser::TOKENR_MESH_CFACE:
			break;
		case ASEParser::TOKENR_MAP_SHINESTRENGTH:
			break;
		case ASEParser::TOKENR_MAP_FILTERCOLOR:
			break;
		case ASEParser::TOKENR_NODE_VISIBILITY_TRACK:
			break;
		case ASEParser::TOKENR_CONTROL_FLOAT_SAMPLE:
			break;
		case ASEParser::TOKENR_CONTROL_FLOAT_KEY:
			break;
		case ASEParser::TOKENR_BONE_PROPERTY:
			break;
		case ASEParser::TOKENR_BONE:
			break;
		case ASEParser::TOKENR_SKIN_INITTM:
			break;
		case ASEParser::TOKENR_CONTROL_SCALE_TRACK:
			break;
		case ASEParser::TOKENR_CONTROL_SCALE_SAMPLE:
			break;
		case ASEParser::TOKENR_SCENE_AMBIENT_ANIM:
			break;
		case ASEParser::TOKENR_CONTROL_POINT3_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_TCB_POINT3_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_TCB_FLOAT_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_TCB_POS_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_TCB_SCALE_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_BEZIER_FLOAT_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_BEZIER_POINT3_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_BEZIER_SCALE_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_POS_LINEAR:
			break;
		case ASEParser::TOKENR_CONTROL_POS_TCB:
			break;
		case ASEParser::TOKENR_CONTROL_ROT_LINEAR:
			break;
		case ASEParser::TOKENR_CONTROL_ROT_BEZIER:
			break;
		case ASEParser::TOKENR_CONTROL_SCALE_LINEAR:
			break;
		case ASEParser::TOKENR_CONTROL_SCALE_TCB:
			break;
		case ASEParser::TOKENR_CONTROL_SCALE_BEZIER:
			break;
		case ASEParser::TOKENR_CONTROL_POS_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_ROT_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_SCALE_KEY:
			break;
		case ASEParser::TOKENR_CONTROL_POINT3_TCB:
			break;
		case ASEParser::TOKENR_CONTROL_POINT3_BEZIER:
			break;
		case ASEParser::TOKENR_CONTROL_COLOR_BEZIER:
			break;
		case ASEParser::TOKENR_CONTROL_POINT3_SAMPLE:
			break;
		case ASEParser::TOKENR_CONTROL_FLOAT_TCB:
			break;
		case ASEParser::TOKENR_CONTROL_FLOAT_BEZIER:
			break;
		case ASEParser::TOKENR_CONTROL_FLOAT_LINEAR:
			break;
		case ASEParser::TOKENR_CAMERA_ANIMATION:
			break;
		case ASEParser::TOKENR_LIGHT_ANIMATION:
			break;
		case ASEParser::TOKENR_MATERIAL_WIREUNITS:
			break;
		case ASEParser::TOKENR_SCENE_MESHFRAMESTEP:
			break;
		case ASEParser::TOKENR_SCENE_KEYFRAMESTEP:
			break;
		case ASEParser::TOKENR_PROP_HIDDEN:
			break;
		case ASEParser::TOKENR_PROP_NORENDERABLE:
			break;
		case ASEParser::TOKENR_PROP_BONE:
			break;
		case ASEParser::TOKENR_END:
			continue;
			break;
		default:
			MessageBox(0, L"something wrong token", 0, 0);
			break;
		}
	}
	_meshes[_meshIndex]->vertices = _vertices;
	_meshes[_meshIndex]->indices = _indices;
	UpdateLocalTransform();
	for (auto obj : _gameObjects)
	{
		if (obj->parent == nullptr)
		{
			_rootObjects.push_back(obj);
		}
	}
}

void ASEParser::Parser::Clear()
{
	_meshIndex = 0;

	_meshes.clear();
	_vertices.clear();
	_verticesOrigin.clear();
	_indices.clear();
	_indicesOrigin.clear();
	_materials.clear();
	_textureIndices.clear();
	_textureUV.clear();
	_gameObjects.clear();
	_rootObjects.clear();

	_nodeMatrix = Matrix::Identity;
}

void ASEParser::Parser::ObjectClear()
{
	_textureIndices.clear();
	_textureUV.clear();
	_vertices.clear();
	_verticesOrigin.clear();
	_indices.clear();
	_indicesOrigin.clear();
}

void ASEParser::Parser::UpdateLocalTransform()
{
	for (auto obj : _gameObjects)
	{
		Matrix inverse = XMMatrixInverse(nullptr, obj->matrix);
		for (auto trans : obj->children)
		{
			Matrix local = trans->matrix * inverse;
			trans->matrix = local;
		}
	}
}

void ASEParser::Parser::ReverseClockWise()
{
	for (uint32 i = 0; i < _indices.size(); i += 3)
	{
		int32 temp = _indices[i];
		_indices[i] = _indices[i + 1];
		_indices[i + 1] = temp;
	}
}
