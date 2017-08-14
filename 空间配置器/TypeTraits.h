#pragma once

struct TrueType
{};

struct FalseType
{};

template<class type>
struct TypeTraits
{
	
	typedef TrueType  this_dummy_member_must_be_first;
	typedef FalseType has_trivial_default_constructor;
	typedef FalseType has_trivial_copy_constructor;
	typedef FalseType has_trivial_assigment_operator;
	typedef FalseType has_trivial_destructor;
	typedef FalseType is_POD_type;
};

template<>
struct TypeTraits<char>
{
	typedef TrueType  this_dummy_member_must_be_first;
	typedef TrueType has_trivial_default_constructor;
	typedef TrueType has_trivial_copy_constructor;
	typedef TrueType has_trivial_assigment_operator;
	typedef TrueType has_trivial_destructor;
	typedef TrueType is_POD_type;
};
template<>
struct TypeTraits<int>
{
	typedef TrueType  this_dummy_member_must_be_first;
	typedef TrueType has_trivial_default_constructor;
	typedef TrueType has_trivial_copy_constructor;
	typedef TrueType has_trivial_assigment_operator;
	typedef TrueType has_trivial_destructor;
	typedef TrueType is_POD_type;
};
template<>
struct TypeTraits<long long>
{
	typedef TrueType  this_dummy_member_must_be_first;
	typedef TrueType has_trivial_default_constructor;
	typedef TrueType has_trivial_copy_constructor;
	typedef TrueType has_trivial_assigment_operator;
	typedef TrueType has_trivial_destructor;
	typedef TrueType is_POD_type;
};
template<>
struct TypeTraits<short>
{
	typedef TrueType  this_dummy_member_must_be_first;
	typedef TrueType has_trivial_default_constructor;
	typedef TrueType has_trivial_copy_constructor;
	typedef TrueType has_trivial_assigment_operator;
	typedef TrueType has_trivial_destructor;
	typedef TrueType is_POD_type;
};
template<>
struct TypeTraits<float>
{
	typedef TrueType  this_dummy_member_must_be_first;
	typedef TrueType has_trivial_default_constructor;
	typedef TrueType has_trivial_copy_constructor;
	typedef TrueType has_trivial_assigment_operator;
	typedef TrueType has_trivial_destructor;
	typedef TrueType is_POD_type;
};
template<>
struct TypeTraits<double>
{
	typedef TrueType  this_dummy_member_must_be_first;
	typedef TrueType has_trivial_default_constructor;
	typedef TrueType has_trivial_copy_constructor;
	typedef TrueType has_trivial_assigment_operator;
	typedef TrueType has_trivial_destructor;
	typedef TrueType is_POD_type;
};
