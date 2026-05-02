#ifndef ENGINE_DEFINE_H_INCLUDED
#define ENGINE_DEFINE_H_INCLUDED


#include "Core/define.h"


#define DECLARE_COMPONENT( name, base ) struct name: base<name, Component::name>
#define DECLARE_COMPONENT_REQUIRE( name, base, require ) struct name: base<name, Component::name, Component::require>
#define DECLARE_COMPONENT_FAMILY( name, base, family ) struct name: base<name, Component::name, Component::NoneMask, Component::family>
#define DECLARE_COMPONENT_REQUIRE_FAMILY( name, base, require, family ) struct name: base<name, Component::name, Component::require, Component::family>

#define DECLARE_COMPONENT_BASE( name, base ) template <class C, Component::TypeEnum typeId, uint64 requireMask = Component::NoneMask, uint64 familyMask = Component::NoneMask> struct name: base<C, typeId, requireMask, familyMask>


#endif