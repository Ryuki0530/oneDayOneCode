import javax.sound.sampled.*;
import java.io.*;

//テスト用の音声作成スクリプト
//LLMによる作成

public class TestWavGenerator {
    public static void main(String[] args) {
        try {
            // パラメータ設定
            float sampleRate = 44100; // サンプリング周波数
            int duration = 2;         // 秒
            double freq = 440.0;      // 周波数(Hz) A=ラの音

            byte[] buf = new byte[(int) sampleRate * duration * 2]; // 16bit PCM
            for (int i = 0; i < buf.length / 2; i++) {
                double angle = 2.0 * Math.PI * i * freq / sampleRate;
                short val = (short) (Math.sin(angle) * 32767); // -32768～32767
                buf[2 * i] = (byte) (val & 0xff);
                buf[2 * i + 1] = (byte) ((val >> 8) & 0xff);
            }

            // WAVフォーマットの指定 (16bit, モノラル)
            AudioFormat format = new AudioFormat(sampleRate, 16, 1, true, false);
            ByteArrayInputStream bais = new ByteArrayInputStream(buf);
            AudioInputStream ais = new AudioInputStream(bais, format, buf.length / format.getFrameSize());

            // ファイルに保存
            File out = new File("sample.wav");
            AudioSystem.write(ais, AudioFileFormat.Type.WAVE, out);

            System.out.println("sample.wav を生成しました！");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
