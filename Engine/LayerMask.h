#pragma once

namespace ATLAT
{
enum class LayerType
{
	Default,
	UI,
};
class LayerMask
{
public:
	uint32 GetValue() { return static_cast<uint32>(_layer); }
	void SetValue(uint32 layer) { _layer = static_cast<LayerType>(layer); }
	void SetValue(LayerType layer) { _layer = layer; }
private:
	LayerType _layer = LayerType::Default;
};
}

