// For examples, see:
// https://thegraybook.vvvv.org/reference/extending/writing-nodes.html#examples

namespace Main;

[ProcessNode]
public class LEDController
{

    private const int LEDROWS = 7;
    private const int LEDCOLS = 18;
    private const int MAXLED = 126;

    private int[,] LED_MATRIX = new int[LEDROWS, LEDCOLS];
    private int[] LED_1D = new int[MAXLED];

    private int colorIndex = 0;
    private int counter = 0;

    // Output array: simulated LED RGB values (flattened)
    public (int r, int g, int b)[] LedBuffer = new (int, int, int)[MAXLED];


    public void InitMapping()
    {
        int index1D = 0;

        for (int r = 0; r < LEDROWS; r++)
        {
            for (int c = 0; c < LEDCOLS; c++)
            {
                int index;
                if (r % 2 == 0)
                {
                    index = r * LEDCOLS + c;
                }
                else
                {
                    index = r * LEDCOLS + (LEDCOLS - 1 - c);
                }

                LED_MATRIX[r, c] = index;
                LED_1D[index1D++] = index;
            }
        }
    }

    // Replace Pixel Hardware: Writes into LedBuffer
    public void SetPixel(int index, int r, int g, int b)
    {
        LedBuffer[index] = (r, g, b);
    }

    public void SetLEDColor(int r, int g, int b)
    {
        for (int i = 0; i < MAXLED; i++)
        {
            SetPixel(i, r, g, b);
        }
    }

    public (int r, int g, int b) ColorWheel(int pos)
    {
        pos = 255 - pos;

        if (pos < 85)
            return (255 - pos * 3, 0, pos * 3);

        if (pos < 170)
        {
            pos -= 85;
            return (0, pos * 3, 255 - pos * 3);
        }

        pos -= 170;
        return (pos * 3, 255 - pos * 3, 0);
    }

    public void UpdateRainbowColor()
    {
        var (r, g, b) = ColorWheel(colorIndex);

        SetLEDColor(r, g, b);

        colorIndex++;
        if (colorIndex >= 255)
            colorIndex = 0;
    }

    public void UpdateNoiseColor(int mode, Func<float, float, float, float> noise)
    {
        float time = Environment.TickCount * 0.1f;

        for (int r = 0; r < LEDROWS; r++)
        {
            for (int c = 0; c < LEDCOLS; c++)
            {
                float n = noise(r * 80, c * 40, time);
                int noiseByte = (int)(n * 255);

                var col = ColorWheel(noiseByte);
                int rr = col.r;
                int gg = col.g;
                int bb = col.b;

                int idx = LED_MATRIX[r, c];

                if (mode == 0)
                {
                    if (gg > 10 && gg < 40)
                        SetPixel(idx, 0, 127, 0);
                    else
                        SetPixel(idx, 0, 0, 0);
                }
                else if (mode == 1)
                {
                    SetPixel(idx, rr, (int)(gg * 0.7f), 0);
                }
            }
        }
    }

    public void ServerAnimationUpdate(int mode, Func<float, float, float, float> noise)
    {
        float time = Environment.TickCount * 0.1f;

        if (mode == 0)
        {
            for (int r = 0; r < LEDROWS; r++)
            {
                for (int c = 0; c < LEDCOLS; c++)
                {
                    float n = noise(r * 80, c * 40, time);
                    int idx = LED_MATRIX[r, c];

                    if (counter % 7 < 3 && n * 255 < 100 + (counter * 0.5f))
                    {
                        if (r % 2 == 0)
                        {
                            if (c % 4 < 1)
                                SetPixel(idx, 255, 255, 0);
                            else
                                SetPixel(idx, 0, 0, 0);
                        }
                        else
                        {
                            if (c % 5 < counter % 5)
                                SetPixel(idx, 0, 255, 0);
                            else
                                SetPixel(idx, 0, 0, 0);
                        }
                    }
                }
            }
        }
        else if (mode == 1)
        {
            for (int i = 0; i < 3; i++)
            {
                int target = counter + i * 40;

                if (target < MAXLED)
                    SetPixel(LED_1D[target], 255, 0, 0);
            }
        }

        counter++;
        if (counter >= MAXLED)
            counter = 0;
    }
}