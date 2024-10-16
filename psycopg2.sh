if [ -f /etc/lsb-release ]; then
    . /etc/lsb-release
    if [ "$DISTRIB_ID" = "Ubuntu" ]; then
        export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
    fi
fi

export PATH=$PATH:/usr/local/pgsql/bin/

mkdir -p build

modulesarray=('psycopgmodule' 'green' 'pqpath' 'utils' 'bytes_format' 'libpq_support' 'win32_support' 'connection_int' 'connection_type' 'cursor_int' 'cursor_type' 'replication_connection_type' 'replication_cursor_type' 'replication_message_type' 'diagnostics_type' 'error_type' 'lobject_int' 'notify_type' 'xid_type' 'adapter_asis' 'adapter_binary' 'adapter_datetime' 'adapter_list' 'adapter_pboolean' 'adapter_pdecimal' 'adapter_pint' 'adapter_pfloat' 'adapter_qstring' 'microprotocols' 'microprotocols_proto' 'typecast' 'lobject_type')

for var in "${modulesarray[@]}"; do
    /usr/local/gcc-4.4.3/bin/gcc -pthread -DPY_FORMAT_LONG_LONG=ll -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -g -Wformat -Werror=format-security -fPIC -DPSYCOPG_DEFAULT_PYDATETIME=1 -DPG_VERSION_NUM=120009 -DHAVE_LO64=1 -I/home/lind/lind_project/tests/applications/python-native-gcc4/python-gcc4/Include/ -I/home/lind/lind_project/tests/applications/python-native-gcc4/python-gcc4/ -I. -I/home/lind/lind_project/tests/applications/postgres -I/home/lind/lind_project/tests/applications/postgres/src/include -I/home/lind/lind_project/tests/applications/postgres/src/interfaces/libpq -c psycopg/$var.c -o build/$var.o -Wdeclaration-after-statement
done

/usr/local/gcc-4.4.3/bin/gcc -o lib/_psycopg.so -pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -Wl,-z,relro -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 -Wall -Wstrict-prototypes -Wdate-time -D_FORTIFY_SOURCE=2 -g -Wformat -Werror=format-security -Wl,-Bsymbolic-functions -Wl,-z,relro -fPIC build/psycopgmodule.o build/green.o build/pqpath.o build/utils.o build/bytes_format.o build/libpq_support.o build/win32_support.o build/connection_int.o build/connection_type.o build/cursor_int.o build/cursor_type.o build/replication_connection_type.o build/replication_cursor_type.o build/replication_message_type.o build/diagnostics_type.o build/error_type.o build/lobject_int.o build/lobject_type.o build/notify_type.o build/xid_type.o build/adapter_asis.o build/adapter_binary.o build/adapter_datetime.o build/adapter_list.o build/adapter_pboolean.o build/adapter_pdecimal.o build/adapter_pint.o build/adapter_pfloat.o build/adapter_qstring.o build/microprotocols.o build/microprotocols_proto.o build/typecast.o -L/home/lind/lind_project/tests/applications/python-native-gcc4/python-gcc4 -L. -L/home/lind/lind_project/tests/applications/postgres/src/interfaces/libpq -lpq -lpython2.7