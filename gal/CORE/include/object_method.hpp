#pragma once

#ifndef GAL_LANG_OBJECT_METHOD_HPP
#define GAL_LANG_OBJECT_METHOD_HPP

#include <object.hpp>

/**
 * @file object_method.hpp
 * @brief This is about the type 'builtin_function_or_method', not GAL methods.
 */

namespace gal::lang
{
	class gal_type_object_internal_method : public gal_type_object
	{
	public:
		struct object_life_manager : traits::object_life_interface<gal_type_object_internal_method>
		{
			struct deallocate_type
			{
				static gal_object* call(host_class_type& self);
			};
		};

		struct object_traverse_manager : traits::object_traverse_interface<gal_type_object_internal_method>
		{
			struct traverse_type
			{
				static bool call(host_class_type& self);
			};
		};

		struct object_represent_manager : traits::object_represent_interface<gal_type_object_internal_method>
		{
			struct represent_type : std::true_type
			{
				static gal_object* call(host_class_type& self);
			};
		};

		struct object_hash_manager : traits::object_hash_interface<gal_type_object_internal_method>
		{
			struct hash_type : std::true_type
			{
				static gal_hash_type call(host_class_type& self);
			};
		};

		struct object_invoke_manager : traits::object_invoke_interface<gal_type_object_internal_method>
		{
			struct invoke_type : std::true_type
			{
				static gal_object* call(host_class_type& self, gal_object* args, gal_object* pair_args);
			};
		};

		struct object_attribute_manager : traits::object_attribute_interface<gal_type_object_internal_method>
		{
			struct object_get_type : std::true_type
			{
				static gal_object* call(host_class_type& self, gal_object& name);
			};
		};

		struct object_compare_manager : traits::object_compare_interface<gal_type_object_internal_method>
		{
			struct compare_type : std::true_type
			{
				static gal_object* compare(gal_object& lhs, gal_object& rhs, compare_operand operand);
			};
		};

	private:
		gal_type_object_internal_method();

	public:
		static gal_type_object_internal_method& type();
	};

	struct gal_method_define
	{
		using internal_function_type = gal_object*(*)(gal_object& self, gal_object* args);
		using internal_function_fast_type = gal_object*(*)(gal_object& self, const gal_object* const* args, gal_size_type num_args);
		using internal_function_pair_arg_type = gal_object*(*)(gal_object& self, gal_object* args, gal_object* pair_args);
		using internal_function_fast_pair_arg_type = gal_object*(*)(gal_object& self, const gal_object* const* args, gal_size_type num_args, gal_object* pair_args);

		using flag_type = std::uint32_t;

		constexpr static flag_type flag_varargs = flag_type{1} << 0;
		constexpr static flag_type flag_pair_args = flag_type{1} << 1;
		// flag_no_arg and flag_an_object_arg must not be combined with the flags above.
		constexpr static flag_type flag_no_args = flag_type{1} << 2;
		constexpr static flag_type flag_an_object_arg = flag_type{1} << 3;
		// flag_class and flag_static are a little different; these control
		// the construction of methods for a class.  These cannot be used for
		// functions in modules.
		constexpr static flag_type flag_class = flag_type{1} << 4;
		constexpr static flag_type flag_static = flag_type{1} << 5;
		constexpr static flag_type flag_coexist = flag_type{1} << 6;
		constexpr static flag_type flag_fastcall = flag_type{1} << 7;
		// flag_method means the function stores an
		// additional reference to the class that defines it;
		// both self and class are passed to it.
		// It uses gal_internal_method_object instead of gal_internal_function_object.
		// May not be combined with flag_no_args, flag_an_object_arg,
		// flag_class or flag_static.
		constexpr static flag_type flag_method = flag_type{1} << 8;

		const char* name;
		internal_function_type method;
		flag_type flag;
		const char* doc;
	};
}

#endif // GAL_LANG_OBJECT_METHOD_HPP
