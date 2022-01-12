#pragma once

#ifndef GAL_LANG_VM_TAGGED_METHOD_HPP
#define GAL_LANG_VM_TAGGED_METHOD_HPP

namespace gal::vm_dev
{
	enum class tagged_method_type : std::uint8_t
	{
		index = 0,
		new_index,
		mode,
		named_call,

		// last tag method with `fast` access
		equal,

		plus,
		minus,
		multiply,
		divide,
		pow,

		unary,

		less_than,
		less_equal,

		call,

		type,

		tagged_method_count
	};

	constexpr std::string_view gal_event_name[] =
	{
			{"$index"},
			{"$new_index"},
			{"$mode"},
			{"$named_call"},

			{"$equal"},

			{"$plus"},
			{"$minus"},
			{"$multiply"},
			{"$divide"},
			{"$pow"},

			{"$unary"},

			{"$less_than"},
			{"$less_equal"},

			{"$call"},

			{"$type"}};

	static_assert(std::size(gal_event_name) == static_cast<size_t>(tagged_method_type::tagged_method_count));

	// see gal.hpp -> object_type
	constexpr std::string_view gal_typename[] =
	{
			{"null"},
			{"boolean"},
			{"number"},
			{"string"},
			{"table"},
			{"function"},
			{"user_data"},
			{"thread"}};

	static_assert(std::size(gal_typename) == static_cast<size_t>(object_type::tagged_value_count));
}

#endif // GAL_LANG_VM_TAGGED_METHOD_HPP
