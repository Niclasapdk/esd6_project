out_dir=bin

all: dir ts808_original ts808_nobias ts808_fixed

debug: dir ts808_fixed_debug ts808_nobias_debug

ts808_original:
	gcc -o "${out_dir}/ts808_original" -lm ts808_original.c

ts808_nobias:
	gcc -o "${out_dir}/ts808_nobias" -lm ts808_nobias.c

ts808_nobias_debug:
	gcc -o "${out_dir}/ts808_nobias" -g -lm ts808_nobias.c -DFUNCTION_GENERATOR

ts808_fixed:
	gcc -o "${out_dir}/ts808_fixed" -lm ts808_fixed.c

ts808_fixed_debug:
	gcc -o "${out_dir}/ts808_fixed" -g -lm ts808_fixed.c -DFUNCTION_GENERATOR -DDEBUG_PRINT

dir:
	mkdir -p ${out_dir}
