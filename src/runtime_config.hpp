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
    static inline int min_mass = 6;
    static inline int min_seq = 2;
    static inline int max_seq = -1;
};
