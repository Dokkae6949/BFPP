#include <stdio.h>
#include <errno.h>

#define MEMORY_SIZE 30000
#define SOURCE_SIZE 30000

char memory[MEMORY_SIZE] = {0};
char source[SOURCE_SIZE];
char *src = source;
char *mem = memory;
char inFunc = 0;


int isLetter(char x) { return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'); }

void brainfuck(char *start, char *end)
{
    src = start;
    size_t loop;
    
    while (*src && src < end)
    {
        switch (*src)
        {
            case '<': mem--; break;
            case '>': mem++; break;
            case '+': ++*mem; break;
            case '-': --*mem; break;
            case '.': fputc(*mem, stdout); break;
            case '#': printf("%d", (int)*mem); break;
            case ',': scanf(" %c", mem); break;
            case '[': 
                if (*mem) break; // if not 0
                loop = 1;
                while (loop)
                {
                    src++;
                    if (*src == '[') loop++;
                    if (*src == ']') loop--;
                }
                break;
            case ']':
                loop = 1;
                while (loop)
                {
                    src--;
                    if (*src == '[') loop--;
                    if (*src == ']') loop++;
                }
                src--;
                break;
            case '{':
                if (isLetter(*(src+1)) && *(src+2) == '}')
                {
                    inFunc = 1;

                    char funName = *(src+1);
                    char *p = src+2;
                    char *fStart;

                    int loop = 1;
                    while (loop && src > start)
                    {
                        src--;
                        if (*src != '{') continue;
                        if (*(src-1) != funName) continue;
                        fStart = src+1;
                        while (loop)
                        {
                            src++;
                            if (*src == '{') loop++;
                            if (*src == '}') loop--;
                        }
                    }
                    brainfuck(fStart, src);
                    src = p;
                    inFunc = 0;
                }
                else
                {
                    loop = 1;
                    while (loop)
                    {
                        src++;
                        if (*src == '{') loop++;
                        if (*src == '}') loop--;
                    }
                    src--;
                }
                
                break;
        }
        
        src++;
    }
}

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        FILE *file = fopen(argv[1], "r");
    
        if(errno)
        {
            printf("File '%s' doesn't exist!\n", argv[1]);
            return 0;
        }

        fread(source, 1, SOURCE_SIZE, file);
        fclose(file);
        brainfuck(source, source + SOURCE_SIZE);
    }
    
    return 0;
}