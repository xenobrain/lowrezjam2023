#include <engine/sound.h>

#include <SDL.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

void data_callback(ma_device* pDevice, void* pOutput, const void*, ma_uint32 frameCount) {
    auto* pDecoder = reinterpret_cast<ma_decoder*>(pDevice->pUserData);
    if (!pDecoder) return;

    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
}

ma_device device;
ma_decoder decoder;

namespace xc::sound {
    auto initialize() -> bool {
        auto file = "../assets/HyperspacePanic.mp3";
        if (ma_decoder_init_file(file, nullptr, &decoder) != MA_SUCCESS) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Could not load file: %s", file);
            return false;
        }

        // Set looping state
        ma_data_source_set_looping(&decoder, MA_TRUE);

        auto device_config = ma_device_config_init(ma_device_type_playback);
        device_config.playback.format = decoder.outputFormat;
        device_config.playback.channels = decoder.outputChannels;
        device_config.sampleRate = decoder.outputSampleRate;
        device_config.dataCallback = data_callback;
        device_config.pUserData = &decoder;

        if (ma_device_init(nullptr, &device_config, &device) != MA_SUCCESS) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Could not init audio device");
            ma_decoder_uninit(&decoder);
            return false;
        }

        if (ma_device_start(&device) != MA_SUCCESS) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Could start audio device");
            ma_device_uninit(&device);
            ma_decoder_uninit(&decoder);
            return false;
        }

        return true;
    }

    auto uninitialize() -> void {
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
    }
} // namespace xc::sound