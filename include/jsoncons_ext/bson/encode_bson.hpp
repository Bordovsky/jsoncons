// Copyright 2013-2023 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_BSON_ENCODE_BSON_HPP
#define JSONCONS_BSON_ENCODE_BSON_HPP

#include <string>
#include <vector>
#include <memory>
#include <type_traits> // std::enable_if
#include <istream> // std::basic_istream
#include <jsoncons/json.hpp>
#include <jsoncons/config/jsoncons_config.hpp>
#include <jsoncons_ext/bson/bson_encoder.hpp>
#include <jsoncons_ext/bson/bson_reader.hpp>

namespace jsoncons { 
namespace bson {

    template<class T, class Container>
    typename std::enable_if<traits_extension::is_basic_json<T>::value &&
                            traits_extension::is_back_insertable_byte_container<Container>::value,void>::type 
    encode_bson(const T& j, 
                Container& v, 
                const bson_encode_options& options = bson_encode_options())
    {
        using char_type = typename T::char_type;
        basic_bson_encoder<jsoncons::bytes_sink<Container>> encoder(v, options);
        auto adaptor = make_json_visitor_adaptor<basic_json_visitor<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T, class Container>
    typename std::enable_if<!traits_extension::is_basic_json<T>::value &&
                            traits_extension::is_back_insertable_byte_container<Container>::value,void>::type 
    encode_bson(const T& val, 
                Container& v, 
                const bson_encode_options& options = bson_encode_options())
    {
        basic_bson_encoder<jsoncons::bytes_sink<Container>> encoder(v, options);
        std::error_code ec;
        encode_traits<T,char>::encode(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }

    template<class T>
    typename std::enable_if<traits_extension::is_basic_json<T>::value,void>::type 
    encode_bson(const T& j, 
                std::ostream& os, 
                const bson_encode_options& options = bson_encode_options())
    {
        using char_type = typename T::char_type;
        bson_stream_encoder encoder(os, options);
        auto adaptor = make_json_visitor_adaptor<basic_json_visitor<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T>
    typename std::enable_if<!traits_extension::is_basic_json<T>::value,void>::type 
    encode_bson(const T& val, 
                std::ostream& os, 
                const bson_encode_options& options = bson_encode_options())
    {
        bson_stream_encoder encoder(os, options);
        std::error_code ec;
        encode_traits<T,char>::encode(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }
  
    // with temp_allocator_rag

    template<class T, class Container, class TempAllocator>
    typename std::enable_if<traits_extension::is_basic_json<T>::value &&
                            traits_extension::is_back_insertable_byte_container<Container>::value,void>::type 
    encode_bson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                const T& j, 
                Container& v, 
                const bson_encode_options& options = bson_encode_options())
    {
        using char_type = typename T::char_type;
        basic_bson_encoder<jsoncons::bytes_sink<Container>,TempAllocator> encoder(v, options, temp_alloc);
        auto adaptor = make_json_visitor_adaptor<basic_json_visitor<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T, class Container, class TempAllocator>
    typename std::enable_if<!traits_extension::is_basic_json<T>::value &&
                            traits_extension::is_back_insertable_byte_container<Container>::value,void>::type 
    encode_bson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                const T& val, 
                Container& v, 
                const bson_encode_options& options = bson_encode_options())
    {
        basic_bson_encoder<jsoncons::bytes_sink<Container>,TempAllocator> encoder(v, options, temp_alloc);
        std::error_code ec;
        encode_traits<T,char>::encode(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }

    template<class T,class TempAllocator>
    typename std::enable_if<traits_extension::is_basic_json<T>::value,void>::type 
    encode_bson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                const T& j, 
                std::ostream& os, 
                const bson_encode_options& options = bson_encode_options())
    {
        using char_type = typename T::char_type;
        basic_bson_encoder<jsoncons::binary_stream_sink,TempAllocator> encoder(os, options, temp_alloc);
        auto adaptor = make_json_visitor_adaptor<basic_json_visitor<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T,class TempAllocator>
    typename std::enable_if<!traits_extension::is_basic_json<T>::value,void>::type 
    encode_bson(temp_allocator_arg_t, const TempAllocator& temp_alloc,
                const T& val, 
                std::ostream& os, 
                const bson_encode_options& options = bson_encode_options())
    {
        basic_bson_encoder<jsoncons::binary_stream_sink,TempAllocator> encoder(os, options, temp_alloc);
        std::error_code ec;
        encode_traits<T,char>::encode(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }
      
} // bson
} // jsoncons

#endif
