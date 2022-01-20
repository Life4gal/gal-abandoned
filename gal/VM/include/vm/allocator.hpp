#pragma once

#ifndef GAL_LANG_VM_ALLOCATOR_HPP
#define GAL_LANG_VM_ALLOCATOR_HPP

#ifndef GAL_ALLOCATOR_NO_TRACE
#include <iostream>// for std::clog
#include <utils/format.hpp>
#include <utils/source_location.hpp>
#endif

#include <vm/memory.hpp>

namespace gal::vm
{
	template<typename T>
	struct vm_allocator
	{
		using allocator_type = std::allocator<T>;
		using allocator_traits = std::allocator_traits<allocator_type>;

		using value_type = typename allocator_traits::value_type;
		using pointer = typename allocator_traits::pointer;
		using size_type = typename allocator_traits::size_type;

		main_state& state;
		[[no_unique_address]] allocator_type dummy_allocator{};

		template<typename U>
		[[nodiscard]] constexpr explicit operator vm_allocator<U>() const noexcept { return {state}; }

		[[nodiscard]] constexpr auto allocate(
				size_type n
				#ifndef GAL_ALLOCATOR_NO_TRACE
				,
				const std_source_location& location = std_source_location::current()
				#endif
				)
		{
			#ifndef GAL_ALLOCATOR_NO_TRACE
			std::clog << std_format::format(
					"Allocate {:>{}} object(s) ({:>{}} byte(s) per object, totally: {:>{}} bytes).",
					n,
					raw_memory::object_amount_width,
					sizeof(value_type),
					raw_memory::memory_use_width,
					sizeof(value_type) * n,
					raw_memory::memory_use_width);
			return static_cast<T*>(raw_memory::allocate(
					state,
					sizeof(value_type) * n,
					location));
			#else
					return static_cast<T*>(raw_memory::allocate(state, sizeof(value_type) * n);
			#endif
		}

		constexpr void deallocate(
				T* p,
				size_type n
				#ifndef GAL_ALLOCATOR_NO_TRACE
				,
				const std_source_location& location = std_source_location::current()
				#endif
				)
		{
			#ifndef GAL_ALLOCATOR_NO_TRACE
			std::clog << std_format::format(
					"Deallocate {:>{}} object(s) at {:>{}} ({:>{}} byte(s) per object, totally: {:>{}} bytes).",
					n,
					raw_memory::object_amount_width,
					static_cast<void*>(p),
					raw_memory::pointer_address_width,
					sizeof(value_type),
					raw_memory::memory_use_width,
					sizeof(value_type) * n,
					raw_memory::memory_use_width);

			raw_memory::deallocate(state, p, sizeof(value_type) * n, location);
			#else
			raw_memory::deallocate(state, p, sizeof(value_type) * n);
			#endif
		}

		template<typename U, typename... Args>
		constexpr void construct(U* p, Args&&... args) { allocator_traits::construct(dummy_allocator, p, std::forward<Args>(args)...); }

		template<typename U>
		constexpr void destroy(U* p) { allocator_traits::destroy(dummy_allocator, p); }

		friend constexpr bool operator==(const vm_allocator& lhs, const vm_allocator& rhs) noexcept { return &lhs.state == &rhs.state; }
	};

	template<typename T1, typename T2>
	constexpr bool operator==(const vm_allocator<T1>& lhs, const vm_allocator<T2>& rhs) { return &lhs.state == &rhs.state; };
}// namespace gal::vm

template<typename ValueType>
struct std::allocator_traits<::gal::vm::vm_allocator<ValueType>>
{
	using allocator_type = ::gal::vm::vm_allocator<ValueType>;
	using internal_allocator_traits = typename ::gal::vm::vm_allocator<ValueType>::allocator_traits;

	using value_type = typename internal_allocator_traits::value_type;
	using pointer = typename internal_allocator_traits::pointer;
	using const_pointer = typename internal_allocator_traits::const_pointer;
	using void_pointer = typename internal_allocator_traits::void_pointer;
	using const_void_pointer = typename internal_allocator_traits::const_void_pointer;
	using difference_type = typename internal_allocator_traits::difference_type;
	using size_type = typename internal_allocator_traits::size_type;

	using propagate_on_container_copy_assignment = typename internal_allocator_traits::propagate_on_container_copy_assignment;
	using propagate_on_container_move_assignment = typename internal_allocator_traits::propagate_on_container_move_assignment;
	using propagate_on_container_swap = typename internal_allocator_traits::propagate_on_container_swap;
	using is_always_equal = typename internal_allocator_traits::is_always_equal;

	template<typename T>
	using rebind_alloc = ::gal::vm::vm_allocator<T>;
	template<typename T>
	using rebind_traits = allocator_traits<rebind_alloc<T>>;

	[[nodiscard]] constexpr static pointer allocate(
			allocator_type& a,
			size_type n
			#ifndef GAL_ALLOCATOR_NO_TRACE
			,
			const std_source_location& location = std_source_location::current()
			#endif
			)
	{
		return a.allocate(n
		                  #ifndef GAL_ALLOCATOR_NO_TRACE
		                  ,
		                  location
		                  #endif
				);
	}

	constexpr static void deallocate(
			allocator_type& a,
			pointer p,
			size_type n
			#ifndef GAL_ALLOCATOR_NO_TRACE
			,
			const std_source_location& location = std_source_location::current()
			#endif
			)
	{
		return a.deallocate(
				p,
				n
				#ifndef GAL_ALLOCATOR_NO_TRACE
				,
				location
				#endif
				);
	}

	template<typename T, typename... Args>
	constexpr static void construct(allocator_type& a, T* p, Args&&... args) { a.construct(p, std::forward<Args>(args)...); }

	template<typename T>
	constexpr static void destroy(allocator_type& a, T* p) { a.destroy(p); }

	constexpr static size_type max_size(const allocator_type& a) noexcept { return internal_allocator_traits::max_size(a.dummy_allocator); }

	constexpr static allocator_type select_on_container_copy_construction(const allocator_type& a) { return a; }
};// namespace std

#endif// GAL_LANG_VM_ALLOCATOR_HPP
