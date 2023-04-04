gcc -o synth synth_example.c ../track_parser.c ../vecimg.c ../synth.c ../freq.c ../parser_utils.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

gcc -o editor vecimg_editor_example.c ../track_parser.c ../vecimg.c ../vecimg_editor.c ../synth.c ../freq.c ../parser_utils.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
