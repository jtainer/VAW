gcc -o synth synth_example.c ../track_parser.c ../vecimg_parser.c ../synth.c ../freq.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

gcc -o editor vecimg_editor_example.c ../track_parser.c ../vecimg_parser.c ../vecimg_editor.c ../synth.c ../freq.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
