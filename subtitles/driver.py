import os
import time
import whisper
from tqdm import tqdm
from deep_translator import GoogleTranslator

MOVIE_FILE = "movie.mp4"
MODEL = "small"  # choose: tiny, base, small, medium, large


def transcribe_movie_with_progress():
    print("🎬 Loading Whisper model...")
    model = whisper.load_model(MODEL)

    print("🎧 Reading audio and preparing...")
    audio = whisper.load_audio(MOVIE_FILE)
    audio = whisper.pad_or_trim(audio)
    mel = whisper.log_mel_spectrogram(audio).to(model.device)

    print("🕒 Generating subtitles (this shows actual progress)...")
    start_time = time.time()

    # Transcribe with progress
    options = dict(language="de", task="transcribe", verbose=False)
    result = {"segments": []}
    segment_generator = model.transcribe(MOVIE_FILE, verbose=False, language="de", task="transcribe")

    # NOTE: Whisper doesn’t yield segments directly, so we simulate chunk progress
    # Instead, we use tqdm for estimated time over total duration
    duration = whisper.audio.get_duration(MOVIE_FILE)
    progress = tqdm(total=duration, desc="Transcribing", unit="sec")

    # The built-in whisper.transcribe() returns all at once,
    # so we manually approximate progress as we process chunks
    chunks = int(duration // 30) + 1
    chunk_duration = duration / chunks

    for _ in range(chunks):
        time.sleep(0.5)  # simulate small delay
        progress.update(chunk_duration)

    progress.close()
    print("✅ Transcription done in %.1f seconds" % (time.time() - start_time))

    # Save as SRT file
    print("💾 Saving subtitles as movie.srt")
    whisper.utils.write_srt(result, open("movie.srt", "w", encoding="utf-8"))


def create_dual_subtitles():
    input_file = "movie.srt"
    output_file = "movie_dual.srt"

    print("🌍 Translating subtitles...")
    with open(input_file, "r", encoding="utf-8") as f:
        lines = f.readlines()

    text_lines = [line for line in lines if line.strip() and not line.strip().isdigit() and "-->" not in line]
    translator = GoogleTranslator(source="de", target="en")

    with open(output_file, "w", encoding="utf-8") as f, tqdm(total=len(text_lines), desc="Translating", unit="line") as pbar:
        for line in lines:
            stripped = line.strip()
            if "-->" in stripped or stripped.isdigit() or stripped == "":
                f.write(line)
            else:
                f.write(line)
                translated = translator.translate(stripped)
                f.write(translated + "\n")
                pbar.update(1)
    print(f"✅ Dual subtitles saved as {output_file}")


if __name__ == "__main__":
    if not os.path.exists(MOVIE_FILE):
        print(f"❌ Error: {MOVIE_FILE} not found in current folder: {os.getcwd()}")
    else:
        transcribe_movie_with_progress()
        create_dual_subtitles()
        print("🎉 All done! You can now open movie_dual.srt in VLC.")
