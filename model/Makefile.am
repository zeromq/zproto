zprotodir = @bindir@/zproto
zproto_DATA = zproto_c.gsl \
			  library.gsl

all-local:  
	gsl -q -script:zproto_c zproto_example.xml
	gsl -q -script:zproto_bnf zproto_example.xml
