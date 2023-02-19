/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-oma <aait-oma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 17:01:32 by aait-oma          #+#    #+#             */
/*   Updated: 2023/02/19 17:56:22 by aait-oma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef VECTOR_HPP
# define VECTOR_HPP
# include <iostream>

namespace ft {
	template < class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			vector() {
				
			};
			bool empty() const {}
			~vector(){};
			typedef T 											value_type;
			typedef Alloc										allocator_type;
			typedef typename allocator_type::size_type 			size_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			bool empty() const { return this->_size == 0; };
			size_type size() const { return this->_size; };
			reference front(){
				return this->_array[0];
			};
			const_reference front() const{
				return this->_array[0];
			};
			reference back(){
				return this->_array[this->_size - 1];
			};
			const_reference back() const{
				return this->_array[this->_size - 1];
			};
			reference operator[] (size_type n){
				return this->_array[n];
			};
			const_reference operator[] (size_type n) const{
				return this->_array[n];
			};
			reference at (size_type n){
				if (n >= this->_size)
					throw std::out_of_range("vector");
				return this->_array[n];
			};
			const_reference at (size_type n) const{
				if (n >= this->_size)
					throw std::out_of_range("vector");
				return this->_array[n];
			};
			// size_type max_size() const{
			// 	return std::numeric_limits<difference_type>::max();
			// }
			allocator_type get_allocator() const {
				return this->_alloc;
			};

		private:
			T*				_array;
			allocator_type	_alloc;
			size_type		_size;
			size_type		_capacity;
	};
}
#endif
