
AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
noinst_LTLIBRARIES = 
noinst_PROGRAMS = 
TESTS = 

# Install scripts into /usr/local/bin
zprotodir = @bindir@
dist_zproto_DATA = \
	zproto_codec_c.gsl \
	zproto_codec_c_v1.gsl \
	zproto_codec_go.gsl \
	zproto_codec_php.gsl \
	zproto_codec_java.gsl \
	zproto_codec_clj.gsl \
	zproto_codec_python.gsl \
	zproto_codec_fs.gsl \
	zproto_codec_cs.gsl \
	zproto_server_c.gsl \
	zproto_server_clj.gsl \
	zproto_server_go.gsl \
	zproto_client_c.gsl \
	zproto_bnf.gsl \
	zproto_dot.gsl \
	zproto_lib.gsl \
	zproto_lib_go.gsl \
	zproto_lib_php.gsl \
	zproto_lib_java.gsl \
	zproto_lib_clj.gsl \
	zproto_lib_fs.gsl \
	zproto_lib_cs.gsl

# Produce generated models; do this manually in src directory
code:
	gsl -q -script:zproto_codec_c zproto_example.xml
	gsl -q -script:zproto_codec_go -package_dir:"go/zmq4" -package:"github.com/pebbe/zmq4" zproto_example.xml
	gsl -q -script:zproto_codec_go -package_dir:"go/goczmq" -package:"github.com/zeromq/goczmq" zproto_example.xml
	gsl -q -script:zproto_codec_php -package_dir:php -test_dir:tests -namespace:Zproto zproto_example.xml
	gsl -q -script:zproto_codec_java zproto_example.xml
	gsl -q -script:zproto_codec_clj zproto_example.xml
	gsl -q -script:zproto_codec_python zproto_example.xml
