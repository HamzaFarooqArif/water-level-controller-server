import os
import subprocess
import threading
import time
from deep_translator import GoogleTranslator
from tqdm import tqdm

MOVIE_FILE = "movie.mp4"     # your movie file name
MODEL = "tiny"              # tiny, base, small, medium, or large


# --- Spinner while Whisper runs ---
def spinner(message, stop_event):
    spinner_cycle = ["|", "/", "-", "\\"]
    i = 0
    start_time = time.time()
    while not stop_event.is_set():
        elapsed = int(time.time() - start_time)
        print(f"\r{message} {spinner_cycle[i % len(spinner_cycle)]} | Elapsed: {elapsed // 60}m {elapsed % 60}s", end="")
        i += 1
        time.sleep(0.2)
    print()  # new line when done


# --- 1️⃣ Step 1: Transcribe German audio using Whisper ---
def transcribe_movie():
    print("🎬 Starting subtitle generation with Whisper...")
    command = [
        "whisper",
        MOVIE_FILE,
        "--language", "German",
        "--task", "transcribe",
        "--output_format", "srt",
        "--model", MODEL
    ]

    stop_event = threading.Event()
    t = threading.Thread(target=spinner, args=("⏳ Generating subtitles...", stop_event))
    t.start()

    start = time.time()
    subprocess.run(command, check=True)
    stop_event.set()
    t.join()

    duration = int(time.time() - start)
    print(f"✅ Transcription complete in {duration // 60}m {duration % 60}s → movie.srt created")


# --- 2️⃣ Step 2: Translate & combine subtitles ---
def create_dual_subtitles():
    input_file = MOVIE_FILE.rsplit(".", 1)[0] + ".srt"
    output_file = MOVIE_FILE.rsplit(".", 1)[0] + "_dual.srt"

    print("🌍 Translating subtitles (German → English)...")
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
                f.write(line)  # German
                translated = translator.translate(stripped)
                f.write(translated + "\n")
                pbar.update(1)

    print(f"\n✅ Dual subtitles saved as {output_file}")


# --- MAIN ---
if __name__ == "__main__":
    if not os.path.exists(MOVIE_FILE):
        print(f"❌ Error: {MOVIE_FILE} not found in current folder: {os.getcwd()}")
    else:
        transcribe_movie()
        create_dual_subtitles()
        print("🎉 All done! You can now open the dual subtitle file in VLC.")
