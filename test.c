#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main() {
    FILE *audio_file, *audio_player;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Open the audio file in binary mode (adjust the path to your file)
    audio_file = fopen("audio.wav", "rb");  // Adjust the filename and path as needed
    if (!audio_file) {
        perror("Failed to open audio file");
        return 1;
    }

    // Open a pipe to ffplay to play the audio data
    audio_player = popen("ffplay -f u8 -ar 44100 -", "w");  // Adjust ffplay options as needed
    if (!audio_player) {
        perror("Failed to open audio player");
        fclose(audio_file);
        return 1;
    }

    // Read from the audio file and write to the audio player
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, audio_file)) > 0) {
        fwrite(buffer, 1, bytes_read, audio_player);
        fflush(audio_player);
    }

    // Cleanup
    pclose(audio_player);
    fclose(audio_file);

    printf("Audio playback completed.\n");
    return 0;
}
