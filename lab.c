#include <stdio.h>
#include <stdlib.h>

int main() {
    
    char *sent = NULL, **words = NULL;
    int i, wordCounter = 0, oldLen = 0;
    
    for(i = 0;; i++){
        sent = (char*)malloc((i + 1) * sizeof(char));
        scanf("%c", sent + i);
        
        if(sent[i] == '.'){
            wordCounter++;
            break;
        }
        
        if(sent[i] == ' '){
            wordCounter++;
            words = (char**)realloc(words, (wordCounter) * sizeof(char*));
            words[wordCounter - 1] = (char*)malloc((i - oldLen) * sizeof(char));
            oldLen = i;
//			memcpy(*(words + i), (sent + i), (i - oldLen));
        }
    }
    
    for(i = 0; i < wordCounter; i++){
        printf("%s", words[i]);
    }
}

