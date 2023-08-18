# 静的ライブラリ vs 動的ライブラリ

## エラーログ

静的ライブラリの`boost_log`をリンクすると以下のようなログが出ることがある。

```log
build exe workspace
g++ -v -Isrc -o bin/workspace obj/Logger/ConsoleSink.o obj/Logger/FileSink.o obj/Logger/GlobalLogger.o obj/Logger/System.o obj/main.o -L/usr/local/lib -lstdc++ -lpthread -lboost_thread -lboost_log -lboost_log_setup -lboost_system -lboost_atomic
Using built-in specs.
COLLECT_GCC=g++
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/11/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 11.4.0-1ubuntu1~22.04' --with-bugurl=file:///usr/share/doc/gcc-11/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,m2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-11 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-bootstrap --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --enable-cet --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-gcn/usr --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --with-build-config=bootstrap-lto-lean --enable-link-serialization=2
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04) 
COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-v' '-I' 'src' '-o' 'bin/workspace' '-L/usr/local/lib' '-shared-libgcc' '-mtune=generic' '-march=x86-64' '-dumpdir' 'bin/workspace.'
 /usr/lib/gcc/x86_64-linux-gnu/11/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/11/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/11/lto-wrapper -plugin-opt=-fresolution=/tmp/ccIacPta.res -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc --build-id --eh-frame-hdr -m elf_x86_64 --hash-style=gnu --as-needed -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie -z now -z relro -o bin/workspace /usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu/Scrt1.o /usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/11/crtbeginS.o -L/usr/local/lib -L/usr/lib/gcc/x86_64-linux-gnu/11 -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/11/../../.. obj/Logger/ConsoleSink.o obj/Logger/FileSink.o obj/Logger/GlobalLogger.o obj/Logger/System.o obj/main.o -lstdc++ -lpthread -lboost_thread -lboost_log -lboost_log_setup -lboost_system -lboost_atomic -lstdc++ -lm -lgcc_s -lgcc -lc -lgcc_s -lgcc /usr/lib/gcc/x86_64-linux-gnu/11/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu/crtn.o
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `Log::Console::CreateBacksink()':
/workspace/src/Logger/ConsoleSink.cpp:41: undefined reference to `boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>::add_stream(boost::shared_ptr<std::ostream> const&)'
/usr/bin/ld: /workspace/src/Logger/ConsoleSink.cpp:42: undefined reference to `boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>::auto_flush(bool)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `Log::Console::ConfigureSink()':
/workspace/src/Logger/ConsoleSink.cpp:85: undefined reference to `boost::log::v2s_mt_posix::core::get()'
/usr/bin/ld: /workspace/src/Logger/ConsoleSink.cpp:85: undefined reference to `boost::log::v2s_mt_posix::core::add_sink(boost::shared_ptr<boost::log::v2s_mt_posix::sinks::sink> const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::attribute_name::attribute_name(char const*)':
/usr/include/boost/log/attributes/attribute_name.hpp:80: undefined reference to `boost::log::v2s_mt_posix::attribute_name::get_id_from_string(char const*)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::once_block_sentry::~once_block_sentry()':
/usr/include/boost/log/utility/once_block.hpp:86: undefined reference to `boost::log::v2s_mt_posix::aux::once_block_sentry::rollback()'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::once_block_sentry::executed() const':
/usr/include/boost/log/utility/once_block.hpp:91: undefined reference to `boost::log::v2s_mt_posix::aux::once_block_sentry::enter_once_block() const'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::expressions::tag::message::get_name()':
/usr/include/boost/log/expressions/message.hpp:56: undefined reference to `boost::log::v2s_mt_posix::aux::default_attribute_names::message()'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::detail::sp_if_not_array<boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char> >::type boost::make_shared<boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>>()':
/usr/include/boost/smart_ptr/make_shared_object.hpp:256: undefined reference to `boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>::basic_text_ostream_backend()'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::format_description<char> boost::log::v2s_mt_posix::aux::parse_format<char>(char const*)':
/usr/include/boost/log/detail/format.hpp:119: undefined reference to `boost::log::v2s_mt_posix::aux::format_description<char> boost::log::v2s_mt_posix::aux::parse_format<char>(char const*, char const*)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `void boost::log::v2s_mt_posix::aux::parse_date_time_format<char, std::char_traits<char>, std::allocator<char> >(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, boost::log::v2s_mt_posix::aux::date_time_format_parser_callback<char>&)':
/usr/include/boost/log/detail/date_time_format_parser.hpp:462: undefined reference to `void boost::log::v2s_mt_posix::aux::parse_date_time_format<char>(char const*, char const*, boost::log::v2s_mt_posix::aux::date_time_format_parser_callback<char>&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::detail::sp_ms_deleter<boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char> >::destroy()':
/usr/include/boost/smart_ptr/make_shared_object.hpp:59: undefined reference to `boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>::~basic_text_ostream_backend()'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::visitation_result boost::log::v2s_mt_posix::value_visitor_invoker<boost::mpl::vector2<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > >, boost::log::v2s_mt_posix::fallback_to_none>::operator()<boost::log::v2s_mt_posix::binder1st<boost::log::v2s_mt_posix::output_fun, boost::log::v2s_mt_posix::expressions::aux::stream_ref<boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> > >&> >(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&, boost::log::v2s_mt_posix::binder1st<boost::log::v2s_mt_posix::output_fun, boost::log::v2s_mt_posix::expressions::aux::stream_ref<boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> > >&>) const':
/usr/include/boost/log/attributes/value_visitation.hpp:182: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_visitation.hpp:183: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_visitation.hpp:191: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::array<std::pair<boost::typeindex::stl_type_index, void*>, 2ul> const& boost::log::v2s_mt_posix::aux::type_sequence_dispatcher<boost::mpl::vector2<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > > >::get_dispatching_map<boost::log::v2s_mt_posix::binder1st<boost::log::v2s_mt_posix::output_fun, boost::log::v2s_mt_posix::expressions::aux::stream_ref<boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> > >&> >()':
/usr/include/boost/log/utility/type_dispatch/static_type_dispatcher.hpp:182: undefined reference to `boost::log::v2s_mt_posix::aux::once_block_sentry::commit()'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::value_extractor<Log::SEVERITY_TYPE, boost::log::v2s_mt_posix::fallback_to_none, Log::tag::severity>::operator()(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&) const':
/usr/include/boost/log/attributes/value_extraction.hpp:232: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:233: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:241: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::value_extractor<int, boost::log::v2s_mt_posix::fallback_to_none, void>::operator()(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&) const':
/usr/include/boost/log/attributes/value_extraction.hpp:232: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:233: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:241: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::value_extractor<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, boost::log::v2s_mt_posix::fallback_to_none, void>::operator()(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&) const':
/usr/include/boost/log/attributes/value_extraction.hpp:232: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:233: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:241: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::value_extractor<boost::mpl::vector2<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > >, boost::log::v2s_mt_posix::fallback_to_none, boost::log::v2s_mt_posix::expressions::tag::message>::operator()(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&) const':
/usr/include/boost/log/attributes/value_extraction.hpp:232: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:233: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:241: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::value_extractor<boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::thread>, boost::log::v2s_mt_posix::fallback_to_none, void>::operator()(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&) const':
/usr/include/boost/log/attributes/value_extraction.hpp:232: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:233: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:241: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::value_extractor<boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process>, boost::log::v2s_mt_posix::fallback_to_none, void>::operator()(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&) const':
/usr/include/boost/log/attributes/value_extraction.hpp:232: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:233: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_extraction.hpp:241: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::array<std::pair<boost::typeindex::stl_type_index, void*>, 2ul> const& boost::log::v2s_mt_posix::aux::type_sequence_dispatcher<boost::mpl::vector2<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > > >::get_dispatching_map<boost::log::v2s_mt_posix::aux::value_ref_initializer<boost::log::v2s_mt_posix::value_ref<boost::mpl::vector2<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > >, boost::log::v2s_mt_posix::expressions::tag::message> > >()':
/usr/include/boost/log/utility/type_dispatch/static_type_dispatcher.hpp:182: undefined reference to `boost::log::v2s_mt_posix::aux::once_block_sentry::commit()'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::visitation_result boost::log::v2s_mt_posix::value_visitor_invoker<boost::posix_time::ptime, boost::log::v2s_mt_posix::fallback_to_none>::operator()<boost::log::v2s_mt_posix::binder1st<boost::log::v2s_mt_posix::aux::light_function<void (boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&, boost::posix_time::ptime const&)> const&, boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&> >(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute_value_set const&, boost::log::v2s_mt_posix::binder1st<boost::log::v2s_mt_posix::aux::light_function<void (boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&, boost::posix_time::ptime const&)> const&, boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&>) const':
/usr/include/boost/log/attributes/value_visitation.hpp:182: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::find(boost::log::v2s_mt_posix::attribute_name) const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_visitation.hpp:183: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::end() const'
/usr/bin/ld: /usr/include/boost/log/attributes/value_visitation.hpp:191: undefined reference to `boost::log::v2s_mt_posix::aux::attach_attribute_name_info(boost::exception&, boost::log::v2s_mt_posix::attribute_name const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::enable_formatting_ostream_generic_operator<boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::thread>, false, boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&>::type boost::log::v2s_mt_posix::operator<< <boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::thread> >(boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::thread> const&)':
/usr/include/boost/log/utility/formatting_ostream.hpp:921: undefined reference to `std::basic_ostream<char, std::char_traits<char> >& boost::log::v2s_mt_posix::aux::operator<< <char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::thread> const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::enable_if_c<(boost::log::v2s_mt_posix::aux::is_character_type<wchar_t>::value&&boost::log::v2s_mt_posix::aux::is_character_type<char>::value)&&((sizeof (wchar_t))!=(sizeof (char))), bool>::type boost::log::v2s_mt_posix::aux::code_convert<wchar_t, char, std::char_traits<char>, std::allocator<char> >(wchar_t const*, unsigned long, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, unsigned long, std::locale const&)':
/usr/include/boost/log/detail/code_conversion.hpp:133: undefined reference to `boost::log::v2s_mt_posix::aux::code_convert_impl(wchar_t const*, unsigned long, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, unsigned long, std::locale const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::enable_formatting_ostream_generic_operator<boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process>, false, boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&>::type boost::log::v2s_mt_posix::operator<< <boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process> >(boost::log::v2s_mt_posix::basic_formatting_ostream<char, std::char_traits<char>, std::allocator<char> >&, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process> const&)':
/usr/include/boost/log/utility/formatting_ostream.hpp:921: undefined reference to `std::basic_ostream<char, std::char_traits<char> >& boost::log::v2s_mt_posix::aux::operator<< <char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process> const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::format_short_year(boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::context&)':
/usr/include/boost/log/detail/decomposed_time.hpp:227: undefined reference to `void boost::log::v2s_mt_posix::aux::put_integer<char>(boost::log::v2s_mt_posix::aux::basic_ostringstreambuf<char, std::char_traits<char>, std::allocator<char> >&, unsigned int, unsigned int, char)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::format_full_year(boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::context&)':
/usr/include/boost/log/detail/decomposed_time.hpp:222: undefined reference to `void boost::log::v2s_mt_posix::aux::put_integer<char>(boost::log::v2s_mt_posix::aux::basic_ostringstreambuf<char, std::char_traits<char>, std::allocator<char> >&, unsigned int, unsigned int, char)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::format_numeric_month(boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::context&)':
/usr/include/boost/log/detail/decomposed_time.hpp:232: undefined reference to `void boost::log::v2s_mt_posix::aux::put_integer<char>(boost::log::v2s_mt_posix::aux::basic_ostringstreambuf<char, std::char_traits<char>, std::allocator<char> >&, unsigned int, unsigned int, char)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `void boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::format_month_day<(char)48>(boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::context&)':
/usr/include/boost/log/detail/decomposed_time.hpp:238: undefined reference to `void boost::log::v2s_mt_posix::aux::put_integer<char>(boost::log::v2s_mt_posix::aux::basic_ostringstreambuf<char, std::char_traits<char>, std::allocator<char> >&, unsigned int, unsigned int, char)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `void boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::format_month_day<(char)32>(boost::log::v2s_mt_posix::aux::date_time_formatter<boost::log::v2s_mt_posix::aux::decomposed_time_wrapper<boost::posix_time::ptime>, char>::context&)':
/usr/include/boost/log/detail/decomposed_time.hpp:238: undefined reference to `void boost::log::v2s_mt_posix::aux::put_integer<char>(boost::log::v2s_mt_posix::aux::basic_ostringstreambuf<char, std::char_traits<char>, std::allocator<char> >&, unsigned int, unsigned int, char)'
/usr/bin/ld: obj/Logger/ConsoleSink.o:/usr/include/boost/log/detail/decomposed_time.hpp:243: more undefined references to `void boost::log::v2s_mt_posix::aux::put_integer<char>(boost::log::v2s_mt_posix::aux::basic_ostringstreambuf<char, std::char_traits<char>, std::allocator<char> >&, unsigned int, unsigned int, char)' follow
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `void boost::log::v2s_mt_posix::sinks::basic_formatting_sink_frontend<char>::feed_record<boost::recursive_mutex, boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char> >(boost::log::v2s_mt_posix::record_view const&, boost::recursive_mutex&, boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>&)':
/usr/include/boost/log/sinks/basic_sink_frontend.hpp:463: undefined reference to `boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>::consume(boost::log::v2s_mt_posix::record_view const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `void boost::log::v2s_mt_posix::sinks::basic_formatting_sink_frontend<char>::feed_record<boost::log::v2s_mt_posix::aux::fake_mutex, boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char> >(boost::log::v2s_mt_posix::record_view const&, boost::log::v2s_mt_posix::aux::fake_mutex&, boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>&)':
/usr/include/boost/log/sinks/basic_sink_frontend.hpp:463: undefined reference to `boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>::consume(boost::log::v2s_mt_posix::record_view const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
/usr/bin/ld: obj/Logger/ConsoleSink.o: in function `void boost::log::v2s_mt_posix::sinks::basic_sink_frontend::flush_backend_impl<boost::recursive_mutex, boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char> >(boost::recursive_mutex&, boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>&, mpl_::bool_<true>)':
/usr/include/boost/log/sinks/basic_sink_frontend.hpp:230: undefined reference to `boost::log::v2s_mt_posix::sinks::basic_text_ostream_backend<char>::flush()'
/usr/bin/ld: obj/Logger/FileSink.o: in function `Log::File::CreateBacksink()':
/workspace/src/Logger/FileSink.cpp:42: undefined reference to `boost::log::v2s_mt_posix::sinks::file::rotation_at_time_point::rotation_at_time_point(unsigned char, unsigned char, unsigned char)'
/usr/bin/ld: /workspace/src/Logger/FileSink.cpp:43: undefined reference to `boost::log::v2s_mt_posix::sinks::text_file_backend::auto_flush(bool)'
/usr/bin/ld: obj/Logger/FileSink.o: in function `Log::File::ConfigureSink()':
/workspace/src/Logger/FileSink.cpp:85: undefined reference to `boost::log::v2s_mt_posix::core::get()'
/usr/bin/ld: /workspace/src/Logger/FileSink.cpp:85: undefined reference to `boost::log::v2s_mt_posix::core::add_sink(boost::shared_ptr<boost::log::v2s_mt_posix::sinks::sink> const&)'
/usr/bin/ld: obj/Logger/FileSink.o: in function `void boost::log::v2s_mt_posix::sinks::text_file_backend::construct<boost::parameter::aux::flat_like_arg_list<boost::parameter::aux::flat_like_arg_tuple<boost::log::v2s_mt_posix::keywords::tag::time_based_rotation, boost::parameter::aux::tagged_argument_rref<boost::log::v2s_mt_posix::keywords::tag::time_based_rotation, boost::log::v2s_mt_posix::sinks::file::rotation_at_time_point>, std::integral_constant<bool, true> >, boost::parameter::aux::flat_like_arg_tuple<boost::log::v2s_mt_posix::keywords::tag::file_name, boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::file_name, char const [23]>, std::integral_constant<bool, true> >, boost::parameter::aux::flat_like_arg_tuple<boost::log::v2s_mt_posix::keywords::tag::rotation_size, boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::rotation_size, int const>, std::integral_constant<bool, true> > > >(boost::parameter::aux::flat_like_arg_list<boost::parameter::aux::flat_like_arg_tuple<boost::log::v2s_mt_posix::keywords::tag::time_based_rotation, boost::parameter::aux::tagged_argument_rref<boost::log::v2s_mt_posix::keywords::tag::time_based_rotation, boost::log::v2s_mt_posix::sinks::file::rotation_at_time_point>, std::integral_constant<bool, true> >, boost::parameter::aux::flat_like_arg_tuple<boost::log::v2s_mt_posix::keywords::tag::file_name, boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::file_name, char const [23]>, std::integral_constant<bool, true> >, boost::parameter::aux::flat_like_arg_tuple<boost::log::v2s_mt_posix::keywords::tag::rotation_size, boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::rotation_size, int const>, std::integral_constant<bool, true> > > const&)':
/usr/include/boost/log/sinks/text_file_backend.hpp:575: undefined reference to `boost::log::v2s_mt_posix::sinks::text_file_backend::construct(boost::filesystem::path const&, boost::filesystem::path const&, std::_Ios_Openmode, unsigned long, boost::log::v2s_mt_posix::aux::light_function<bool ()> const&, boost::log::v2s_mt_posix::sinks::auto_newline_mode, bool, bool)'
/usr/bin/ld: obj/Logger/FileSink.o: in function `boost::detail::sp_ms_deleter<boost::log::v2s_mt_posix::sinks::text_file_backend>::destroy()':
/usr/include/boost/smart_ptr/make_shared_object.hpp:59: undefined reference to `boost::log::v2s_mt_posix::sinks::text_file_backend::~text_file_backend()'
/usr/bin/ld: obj/Logger/FileSink.o: in function `boost::log::v2s_mt_posix::aux::light_function<bool ()>::impl<boost::log::v2s_mt_posix::sinks::file::rotation_at_time_point>::invoke_impl(void*)':
/usr/include/boost/log/detail/light_function.hpp:155: undefined reference to `boost::log::v2s_mt_posix::sinks::file::rotation_at_time_point::operator()() const'
/usr/bin/ld: obj/Logger/FileSink.o: in function `void boost::log::v2s_mt_posix::sinks::basic_formatting_sink_frontend<char>::feed_record<boost::recursive_mutex, boost::log::v2s_mt_posix::sinks::text_file_backend>(boost::log::v2s_mt_posix::record_view const&, boost::recursive_mutex&, boost::log::v2s_mt_posix::sinks::text_file_backend&)':
/usr/include/boost/log/sinks/basic_sink_frontend.hpp:463: undefined reference to `boost::log::v2s_mt_posix::sinks::text_file_backend::consume(boost::log::v2s_mt_posix::record_view const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
/usr/bin/ld: obj/Logger/FileSink.o: in function `void boost::log::v2s_mt_posix::sinks::basic_formatting_sink_frontend<char>::feed_record<boost::log::v2s_mt_posix::aux::fake_mutex, boost::log::v2s_mt_posix::sinks::text_file_backend>(boost::log::v2s_mt_posix::record_view const&, boost::log::v2s_mt_posix::aux::fake_mutex&, boost::log::v2s_mt_posix::sinks::text_file_backend&)':
/usr/include/boost/log/sinks/basic_sink_frontend.hpp:463: undefined reference to `boost::log::v2s_mt_posix::sinks::text_file_backend::consume(boost::log::v2s_mt_posix::record_view const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
/usr/bin/ld: obj/Logger/FileSink.o: in function `void boost::log::v2s_mt_posix::sinks::basic_sink_frontend::flush_backend_impl<boost::recursive_mutex, boost::log::v2s_mt_posix::sinks::text_file_backend>(boost::recursive_mutex&, boost::log::v2s_mt_posix::sinks::text_file_backend&, mpl_::bool_<true>)':
/usr/include/boost/log/sinks/basic_sink_frontend.hpp:230: undefined reference to `boost::log::v2s_mt_posix::sinks::text_file_backend::flush()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attribute::impl::~impl()':
/usr/include/boost/log/attributes/attribute.hpp:75: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::current_process_name::current_process_name()':
/usr/include/boost/log/attributes/current_process_name.hpp:53: undefined reference to `boost::log::v2s_mt_posix::aux::get_process_name[abi:cxx11]()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::current_process_id::current_process_id()':
/usr/include/boost/log/attributes/current_process_id.hpp:49: undefined reference to `boost::log::v2s_mt_posix::aux::this_process::get_id()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::current_thread_id::impl::dispatch(boost::log::v2s_mt_posix::type_dispatcher&)':
/usr/include/boost/log/attributes/current_thread_id.hpp:69: undefined reference to `boost::log::v2s_mt_posix::aux::this_thread::get_id()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::current_thread_id::impl::detach_from_thread()':
/usr/include/boost/log/attributes/current_thread_id.hpp:79: undefined reference to `boost::log::v2s_mt_posix::aux::this_thread::get_id()'
/usr/bin/ld: /usr/include/boost/log/attributes/current_thread_id.hpp:79: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attribute_value::impl::~impl()':
/usr/include/boost/log/attributes/attribute_value.hpp:73: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::current_thread_id::current_thread_id()':
/usr/include/boost/log/attributes/current_thread_id.hpp:89: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::basic_logger<char, boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE>, boost::log::v2s_mt_posix::sources::multi_thread_model<boost::log::v2s_mt_posix::aux::light_rw_mutex> >::~basic_logger()':
/usr/include/boost/log/sources/basic_logger.hpp:81: undefined reference to `boost::log::v2s_mt_posix::attribute_set::~attribute_set()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::constant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >::constant(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&&)':
/usr/include/boost/log/attributes/constant.hpp:84: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::constant<boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process> >::constant(boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process>&&)':
/usr/include/boost/log/attributes/constant.hpp:84: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::basic_clock<boost::log::v2s_mt_posix::attributes::local_time_traits>::basic_clock()':
/usr/include/boost/log/attributes/clock.hpp:71: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::attribute_value_impl<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >::~attribute_value_impl()':
/usr/include/boost/log/attributes/attribute_value_impl.hpp:49: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::attribute_value_impl<boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process> >::~attribute_value_impl()':
/usr/include/boost/log/attributes/attribute_value_impl.hpp:49: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::basic_severity_logger<boost::log::v2s_mt_posix::sources::basic_logger<char, boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE>, boost::log::v2s_mt_posix::sources::multi_thread_model<boost::log::v2s_mt_posix::aux::light_rw_mutex> >, Log::SEVERITY_TYPE>::basic_severity_logger()':
/usr/include/boost/log/sources/severity_feature.hpp:201: undefined reference to `boost::log::v2s_mt_posix::aux::default_attribute_names::severity()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::basic_logger<char, boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE>, boost::log::v2s_mt_posix::sources::multi_thread_model<boost::log::v2s_mt_posix::aux::light_rw_mutex> >::add_attribute_unlocked(boost::log::v2s_mt_posix::attribute_name const&, boost::log::v2s_mt_posix::attribute const&)':
/usr/include/boost/log/sources/basic_logger.hpp:231: undefined reference to `boost::log::v2s_mt_posix::attribute_set::insert(boost::log::v2s_mt_posix::attribute_name, boost::log::v2s_mt_posix::attribute const&)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::basic_logger<char, boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE>, boost::log::v2s_mt_posix::sources::multi_thread_model<boost::log::v2s_mt_posix::aux::light_rw_mutex> >::basic_logger()':
/usr/include/boost/log/sources/basic_logger.hpp:137: undefined reference to `boost::log::v2s_mt_posix::core::get()'
/usr/bin/ld: /usr/include/boost/log/sources/basic_logger.hpp:137: undefined reference to `boost::log::v2s_mt_posix::attribute_set::attribute_set()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::aux::severity_level<Log::SEVERITY_TYPE>::severity_level()':
/usr/include/boost/log/sources/severity_feature.hpp:99: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::aux::severity_level<Log::SEVERITY_TYPE>::impl::~impl()':
/usr/include/boost/log/sources/severity_feature.hpp:67: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::basic_clock<boost::log::v2s_mt_posix::attributes::local_time_traits>::impl::~impl()':
/usr/include/boost/log/attributes/clock.hpp:57: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::constant<boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::process> >::impl::~impl()':
/usr/include/boost/log/attributes/constant.hpp:56: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::constant<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >::impl::~impl()':
/usr/include/boost/log/attributes/constant.hpp:56: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::attribute_value_impl<boost::log::v2s_mt_posix::aux::id<boost::log::v2s_mt_posix::aux::thread> >::~attribute_value_impl()':
/usr/include/boost/log/attributes/attribute_value_impl.hpp:49: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o:/usr/include/boost/log/attributes/current_thread_id.hpp:59: more undefined references to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)' follow
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::aux::severity_level<Log::SEVERITY_TYPE>::impl::dispatch(boost::log::v2s_mt_posix::type_dispatcher&)':
/usr/include/boost/log/sources/severity_feature.hpp:77: undefined reference to `boost::log::v2s_mt_posix::sources::aux::get_severity_level()'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::sources::aux::severity_level<Log::SEVERITY_TYPE>::impl::detach_from_thread()':
/usr/include/boost/log/sources/severity_feature.hpp:89: undefined reference to `boost::log::v2s_mt_posix::sources::aux::get_severity_level()'
/usr/bin/ld: /usr/include/boost/log/sources/severity_feature.hpp:88: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::basic_clock<boost::log::v2s_mt_posix::attributes::local_time_traits>::impl::get_value()':
/usr/include/boost/log/attributes/clock.hpp:63: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::attribute_value_impl<boost::posix_time::ptime>::~attribute_value_impl()':
/usr/include/boost/log/attributes/attribute_value_impl.hpp:49: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/Logger/GlobalLogger.o: in function `boost::log::v2s_mt_posix::attributes::attribute_value_impl<Log::SEVERITY_TYPE>::~attribute_value_impl()':
/usr/include/boost/log/attributes/attribute_value_impl.hpp:49: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::record::reset()':
/usr/include/boost/log/core/record.hpp:157: undefined reference to `boost::log::v2s_mt_posix::record_view::public_data::destroy(boost::log::v2s_mt_posix::record_view::public_data const*)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::record boost::log::v2s_mt_posix::sources::basic_composite_logger<char, boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE>, boost::log::v2s_mt_posix::sources::multi_thread_model<boost::log::v2s_mt_posix::aux::light_rw_mutex>, boost::log::v2s_mt_posix::sources::features<boost::log::v2s_mt_posix::sources::severity<Log::SEVERITY_TYPE> > >::open_record<boost::parameter::aux::tagged_argument_list_of_1<boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::severity, Log::SEVERITY_TYPE const> > >(boost::parameter::aux::tagged_argument_list_of_1<boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::severity, Log::SEVERITY_TYPE const> > const&)':
/usr/include/boost/log/sources/basic_logger.hpp:463: undefined reference to `boost::log::v2s_mt_posix::core::get_logging_enabled() const'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::basic_record_ostream<char>& boost::log::v2s_mt_posix::operator<< <char, int&&>(boost::log::v2s_mt_posix::basic_record_ostream<char>&, boost::log::v2s_mt_posix::add_value_manip<int&&> const&)':
/usr/include/boost/log/utility/manipulators/add_value.hpp:99: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: /usr/include/boost/log/utility/manipulators/add_value.hpp:100: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::insert(boost::log::v2s_mt_posix::attribute_name, boost::log::v2s_mt_posix::attribute_value const&)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::basic_record_ostream<char>& boost::log::v2s_mt_posix::operator<< <char, char const (&) [13]>(boost::log::v2s_mt_posix::basic_record_ostream<char>&, boost::log::v2s_mt_posix::add_value_manip<char const (&) [13]> const&)':
/usr/include/boost/log/utility/manipulators/add_value.hpp:99: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: /usr/include/boost/log/utility/manipulators/add_value.hpp:100: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::insert(boost::log::v2s_mt_posix::attribute_name, boost::log::v2s_mt_posix::attribute_value const&)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::basic_record_ostream<char>& boost::log::v2s_mt_posix::operator<< <char, char const (&) [5]>(boost::log::v2s_mt_posix::basic_record_ostream<char>&, boost::log::v2s_mt_posix::add_value_manip<char const (&) [5]> const&)':
/usr/include/boost/log/utility/manipulators/add_value.hpp:99: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator new(unsigned long)'
/usr/bin/ld: /usr/include/boost/log/utility/manipulators/add_value.hpp:100: undefined reference to `boost::log::v2s_mt_posix::attribute_value_set::insert(boost::log::v2s_mt_posix::attribute_name, boost::log::v2s_mt_posix::attribute_value const&)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::aux::lazy_singleton<boost::log::v2s_mt_posix::sources::aux::logger_singleton<Log::g_logger>, boost::shared_ptr<boost::log::v2s_mt_posix::sources::aux::logger_holder<boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE> > > >::get()':
/usr/include/boost/log/detail/singleton.hpp:43: undefined reference to `boost::log::v2s_mt_posix::aux::once_block_sentry::commit()'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::aux::record_pump<boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE> >::record_pump(boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE>&, boost::log::v2s_mt_posix::record&)':
/usr/include/boost/log/sources/record_ostream.hpp:508: undefined reference to `boost::log::v2s_mt_posix::aux::stream_provider<char>::allocate_compound(boost::log::v2s_mt_posix::record&)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::aux::record_pump<boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE> >::auto_release::~auto_release()':
/usr/include/boost/log/sources/record_ostream.hpp:493: undefined reference to `boost::log::v2s_mt_posix::aux::stream_provider<char>::release_compound(boost::log::v2s_mt_posix::aux::stream_provider<char>::stream_compound*)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::sources::aux::logger_singleton<Log::g_logger>::init_instance()':
/usr/include/boost/log/sources/global_logger_storage.hpp:127: undefined reference to `boost::log::v2s_mt_posix::sources::aux::global_storage::get_or_init(boost::typeindex::stl_type_index, boost::shared_ptr<boost::log::v2s_mt_posix::sources::aux::logger_holder_base> (*)())'
/usr/bin/ld: /usr/include/boost/log/sources/global_logger_storage.hpp:143: undefined reference to `boost::log::v2s_mt_posix::sources::aux::throw_odr_violation(boost::typeindex::stl_type_index, boost::typeindex::stl_type_index, boost::log::v2s_mt_posix::sources::aux::logger_holder_base const&)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::sources::aux::severity_level<Log::SEVERITY_TYPE>::set_value(Log::SEVERITY_TYPE)':
/usr/include/boost/log/sources/severity_feature.hpp:137: undefined reference to `boost::log::v2s_mt_posix::sources::aux::get_severity_level()'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::record boost::log::v2s_mt_posix::sources::basic_logger<char, boost::log::v2s_mt_posix::sources::severity_logger_mt<Log::SEVERITY_TYPE>, boost::log::v2s_mt_posix::sources::multi_thread_model<boost::log::v2s_mt_posix::aux::light_rw_mutex> >::open_record_unlocked<boost::parameter::aux::tagged_argument_list_of_1<boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::severity, Log::SEVERITY_TYPE const> > >(boost::parameter::aux::tagged_argument_list_of_1<boost::parameter::aux::tagged_argument<boost::log::v2s_mt_posix::keywords::tag::severity, Log::SEVERITY_TYPE const> > const&)':
/usr/include/boost/log/sources/basic_logger.hpp:263: undefined reference to `boost::log::v2s_mt_posix::core::open_record(boost::log::v2s_mt_posix::attribute_set const&)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::core::push_record(boost::log::v2s_mt_posix::record&&)':
/usr/include/boost/log/core/core.hpp:308: undefined reference to `boost::log::v2s_mt_posix::core::push_record_move(boost::log::v2s_mt_posix::record&)'
/usr/bin/ld: obj/main.o: in function `boost::log::v2s_mt_posix::attributes::attribute_value_impl<int>::~attribute_value_impl()':
/usr/include/boost/log/attributes/attribute_value_impl.hpp:49: undefined reference to `boost::log::v2s_mt_posix::attribute::impl::operator delete(void*, unsigned long)'
collect2: error: ld returned 1 exit status
make: *** [Makefile:42: workspace] Error 1
```

## 理由

内容を見てみると、静的ライブラリの名前空間に記載されている関数が読み取れない模様。

- `v2s_mt_posix`: 静的マルチスレッドライブラリ
- `v2_mt_posix`: 動的マルチスレッドライブラ

> As a couple quick examples, v2s_st corresponds to v2 static single-threaded build of the library and v2_mt_posix - to v2 dynamic multi-threaded build for POSIX system API.

`boost_log`はデフォルトで静的ライブラリをリンクする作りになっているようだ。

静的ライブラリをリンクする場合、フルパスで明示的に指定する必要がある。  
`makefile`を見てみると、特にフルパス指定はしておらず、動的ライブラリ側をリンクしている模様。

```makefile
# compiler setting
COMPILER = g++ -v
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self -std=c++17
# library setting
LDFLAGS = -L/usr/local/lib
LIBS    = -lstdc++ -lpthread -lboost_thread -lboost_log -lboost_log_setup -lboost_system -lboost_atomic
（省略）
```

この食い違いによってライブラリを定義を見失ってしまっているらしい。

## 解決方法

### 動的ライブラリを参照させる

Boostライブラリで、動的リンクをリンクさせるには特殊なdefine定義が必要な模様

- `BOOST_LOG_DYN_LINK`: Boost.Logのライブラリだけ動的ライブラリとしてリンクさせる
- `BOOST_ALL_DYN_LINK`: すべてのBoostライブラリを動的ライブラリとしてリンクさせる

`-D`オプションを使ってdefine定義を追加するとビルドできた。

```makefile
CXXFLAGS = -g -MMD -Wall -Wextra -Winit-self -std=c++17 -DBOOST_LOG_DYN_LINK
```

## 参考

- [undefined reference to boost::log::v2_mt_posix #46](https://github.com/boostorg/log/issues/46#issuecomment-354059893)
- [Why my application fails to link with Boost.Log? What's the fuss about library namespaces?](https://www.boost.org/doc/libs/1_66_0/libs/log/doc/html/log/rationale/namespace_mangling.html)
