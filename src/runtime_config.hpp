#pragma once

class RuntimeConfig final
{
  public:
    enum class CloneType
    {
        type1,
        type2
    };
    static inline CloneType clone_type = CloneType::type1;
};
