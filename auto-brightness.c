#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int verbose = 0;
char *illuminance_path = "/sys/bus/iio/devices/iio:device0/in_illuminance_input";
int set_integration = 0;
char *illuminance_integration_time_path = "/sys/bus/iio/devices/iio:device0/in_illuminance_integration_time";
float illuminance_integration_time = 0.8;
int illuminance_min = 0;
int illuminance_max = 30;
int poll_interval = 800;
int step_const = 10;
int step_min = 100;
int step_max = 1000;
char *brightness_path = "/sys/class/backlight/intel_backlight/brightness";
char *max_brightness_path = "/sys/class/backlight/intel_backlight/max_brightness";
int brightness_interval = 50;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int current_brightness;
int target_brightness;
int previous_target_brightness;

int getValue(char file_path[])
{
    FILE *file;
    int value;
    file = fopen(file_path, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%i", &value) != 1)
    {
        perror("Failed to read value");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return value;
}

void setValue(char file_path[], char value[])
{
    FILE *file;
    file = fopen(file_path, "w");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    if (fprintf(file, "%s", value) < 0)
    {
        perror("Failed to write value");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void *smoothBrightnessControl()
{
    FILE *file;
    file = fopen(brightness_path, "w");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        current_brightness = getValue(brightness_path);
        if (target_brightness != current_brightness)
        {
            int diff = target_brightness - current_brightness;
            int step = diff / step_const;
            if (step > 0)
            {
                if (step < step_min)
                {
                    if (diff < step_min)
                    {
                        step = diff;
                    }
                    else
                    {
                        step = step_min;
                    }
                }
                else if (step > step_max)
                {
                    step = step_max;
                }
            }
            else
            {
                if (step > -step_min)
                {
                    if (diff > -step_min)
                    {
                        step = diff;
                    }
                    else
                    {
                        step = -step_min;
                    }
                }
                else if (step < -step_max)
                {
                    step = -step_max;
                }
            }

            if (verbose == 1)
            {
                printf("Step: %i, Next: %i\n", step, current_brightness + step);
            }
            fprintf(file, "%i", current_brightness + step);
            fflush(file);
            usleep(50 * 1000); // 50ms
        }
        else
        {
            if (verbose == 1)
            {
                printf("No change, waiting for signal...\n");
            }
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cond, &mutex);
            pthread_mutex_unlock(&mutex);
            if (verbose == 1)
            {
                printf("Signal received, resuming...\n");
            }
        }
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            printf("Usage: %s --help --verbose --illuminance-path \"/sys/bus/iio/devices/iio:device0/in_illuminance_input\" --set-integration --illuminance-integration-time-path \"/sys/bus/iio/devices/iio:device0/in_illuminance_integration_time\" --illuminance-integration-time 0.8 --illuminance-min 0 --illuminance-max 30 --poll-interval 800 --step-const 10 --step-min 100 --step-max 1000 --brightness-path \"/sys/class/backlight/intel_backlight/brightness\" --max-brightness-path \"/sys/class/backlight/intel_backlight/max_brightness\" --brightness-interval 50\n"
                   "\nArguments explanation:\n"
                   "--help: Show this help information and exit\n"
                   "--verbose: Show debug information\n"
                   "--illuminance-path: Path to light sensor file\n"
                   "--set-integration: Enables writing integration time value to the sensor\n"
                   "--illuminance-integration-time-path: Path to sensor integration time value file\n"
                   "--illuminance-integration-time: Value to be set as integration time\n"
                   "--illuminance-min: Below this value the brightness will be 0\n"
                   "--illuminance-max: Above this value the brightness will be maxed\n"
                   "--poll-interval: Illuminance sensor poll interval in ms\n"
                   "--step-const: The value to divide the brightness difference by\n"
                   "--step-min: Minimum absolute step value\n"
                   "--step-max: Maximum absolute step value\n"
                   "--brightness-path: Path to display brightness file\n"
                   "--max-brightness-path: Path to max display brightness file\n"
                   "--brightness-interval: Time between brightness steps\n"
                   ,argv[0]);
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], "--verbose") == 0)
        {
            verbose = 1;
        }
        else if (strcmp(argv[i], "--illuminance-path") == 0 && i + 1 < argc)
        {
            illuminance_path = argv[i + 1];
        }
        else if (strcmp(argv[i], "--set-integration") == 0)
        {
            set_integration = 1;
        }
        else if (strcmp(argv[i], "--illuminance-integration-time-path") == 0 && i + 1 < argc)
        {
            illuminance_integration_time_path = argv[i + 1];
        }
        else if (strcmp(argv[i], "--illuminance-integration-time") == 0 && i + 1 < argc)
        {
            illuminance_integration_time = atof(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--illuminance-min") == 0 && i + 1 < argc)
        {
            illuminance_min = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--illuminance-max") == 0 && i + 1 < argc)
        {
            illuminance_max = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--poll-interval") == 0 && i + 1 < argc)
        {
            poll_interval = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--step-const") == 0 && i + 1 < argc)
        {
            step_const = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--step-min") == 0 && i + 1 < argc)
        {
            step_min = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--step-max") == 0 && i + 1 < argc)
        {
            step_max = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--brightness-path") == 0 && i + 1 < argc)
        {
            brightness_path = argv[i + 1];
        }
        else if (strcmp(argv[i], "--max-brightness-path") == 0 && i + 1 < argc)
        {
            max_brightness_path = argv[i + 1];
        }
        else if (strcmp(argv[i], "--brightness-interval") == 0 && i + 1 < argc)
        {
            brightness_interval = atoi(argv[i + 1]);
        }
    }
    printf("Run with --help to show help\n");
    if (verbose == 1)
    {
        printf("\nLaunching with arguments: \n");
        printf("illuminance_path: %s\n", illuminance_path);
        printf("set_integration: %i\n", set_integration);
        printf("illuminance_integration_time_path: %s\n", illuminance_integration_time_path);
        printf("illuminance_integration_time: %f\n", illuminance_integration_time);
        printf("illuminance_min: %i\n", illuminance_min);
        printf("illuminance_max: %i\n", illuminance_max);
        printf("poll_interval: %i\n", poll_interval);
        printf("step_const: %i\n", step_const);
        printf("step_min: %i\n", step_min);
        printf("step_max: %i\n", step_max);
        printf("brightness_path: %s\n", brightness_path);
        printf("max_brightness_path: %s\n\n", max_brightness_path);
    }

    if (set_integration == 1)
    {
        char *illuminance_integration_time_str;
        gcvt(illuminance_integration_time, 4, illuminance_integration_time_str);
        if (verbose == 1)
        {
            printf("Setting %s to %s...\n", illuminance_integration_time_path, illuminance_integration_time_str);
        }
        setValue(illuminance_integration_time_path, illuminance_integration_time_str);
    }

    const int max_brightness = getValue(max_brightness_path);
    current_brightness = getValue(brightness_path);
    target_brightness = current_brightness;
    previous_target_brightness = target_brightness;

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, smoothBrightnessControl, NULL) != 0)
    {
        printf("Failed to create thread\n");
        return 1;
    }

    while (1)
    {
        int current_illuminance = getValue(illuminance_path);

        target_brightness = (current_illuminance - illuminance_min) * max_brightness / (illuminance_max - illuminance_min);
        if (target_brightness > max_brightness)
        {
            target_brightness = max_brightness;
        }
        else if (target_brightness < 0)
        {
            target_brightness = 0;
        };

        if (verbose == 1)
        {
            printf("illuminance: %i, brightness: %i, target: %i", current_illuminance, current_brightness, target_brightness);
        }
        if (previous_target_brightness != target_brightness)
        {
            previous_target_brightness = target_brightness;
            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            if (verbose == 1)
            {
                printf(" (changed!, signal sent!)");
            }
        }
        if (verbose == 1)
        {
            printf("\n");
        }
        usleep(poll_interval * 1000);
    }

    // Wait for the thread to finish
    // pthread_join(thread_id, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    exit(EXIT_SUCCESS);
}