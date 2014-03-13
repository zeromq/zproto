# Generate gsl code
CODE_SCR_XML = myproj_msg.xml

# Make executes each line of a recipe in a seperate shell.
# Hence we need to export and generate in one line.
code: $(CODE_SCR_XML)
	export PATH=@bindir@/zproto:$$PATH; gsl -q -script:zproto_c $<;

