#pragma once

#ifndef GAL_LANG_FOUNDATION_PARAMETERS_HPP
#define GAL_LANG_FOUNDATION_PARAMETERS_HPP

#include <gal/foundation/boxed_value.hpp>
#include <gal/foundation/type_info.hpp>
#include <utils/initializer_list.hpp>
#include <vector>

namespace gal::lang::foundation
{
	using parameters_type = std::vector<boxed_value>;
	using parameters_view_type = utils::initializer_list<boxed_value>;

	using type_infos_type = std::vector<gal_type_info>;
	using type_infos_view_type = utils::initializer_list<gal_type_info>;

	/**
	 * @brief Const version of proxy_function. Points to a const proxy_function.
	 * This is how most registered functions are handled internally.
	 */
	using immutable_proxy_function = std::shared_ptr<const class proxy_function_base>;
	/**
	 * @brief Common typedef used for passing of any registered function in GAL.
	 */
	using mutable_proxy_function = std::shared_ptr<proxy_function_base>;

	using proxy_function = immutable_proxy_function;

	using immutable_proxy_functions_type = std::vector<immutable_proxy_function>;
	using mutable_proxy_functions_type = std::vector<mutable_proxy_function>;
}

#endif // GAL_LANG_FOUNDATION_PARAMETERS_HPP
