#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define min(x, y) (x) < (y)? (x) : (y)
#define COUNT_ARGS 7

typedef u_int64_t count_t;

typedef struct _player{
    count_t scores;
} Player;


typedef struct _params {
    Player first, second;
    count_t experiments;
    count_t moves_count;
    count_t first_won, second_won;
} Params;


typedef struct _command {
    int threads_num;
    int K;
    int tour_now;
    int first_player_score, second_player_score;
    int experiments;
} Command;

enum WON{
    DRAW,
    FIRST,
    SECOND
};


int rand_score(){
    int (*rfunc)(void) = rand;
    return (rfunc() % 6)  + (rfunc() % 6) + 2;
}

int experiment(count_t moves_count, Player first, Player second){
    for (int i = 0; i < moves_count; ++i) {
        first.scores += rand_score();
        second.scores += rand_score();
    }
    if(first.scores > second.scores){
        return FIRST;
    }
    else if(second.scores > first.scores) {
        return SECOND;
    }
    return DRAW;
}

void init(Params *ptr, Command *command)
{
    Player f = {command->first_player_score};
    Player s = {command->second_player_score};
    ptr[0].first = f;
    ptr[0].second = s;

    ptr[0].first_won = ptr[0].second_won = 0;

    ptr[0].experiments = command->experiments / command->threads_num;
    ptr[0].moves_count = (command->K - command->tour_now - 1);
    for (size_t i = 1; i < command->threads_num; ++i) {
        ptr[i].first = ptr[i-1].first;
        ptr[i].second = ptr[i-1].second;
        ptr[i].moves_count = ptr[i - 1].moves_count;
        ptr[i].experiments = ptr[i - 1].experiments;
        ptr[i].first_won = ptr[i].second_won = 0;
    }
    ptr[command->threads_num - 1].experiments += command->experiments % command->threads_num;
}

void *thread_function(void *somePtr){
    srand(time(NULL));
    Params *params = (Params *)somePtr;
    for (int i = 0; i < params->experiments; ++i) {
        int won = experiment(params->moves_count, params->first, params->second);
        if(won == FIRST){
            params->first_won++;
        } else if(won == SECOND){
            params->second_won++;
        }
    }
    return NULL;
}

void parse_command_line(int argc, char **argv, Command *command) {
    if(argc != COUNT_ARGS){
        fprintf(stderr, "%s\n", "usage: threads_number K tour_now first_player_score second_player_score experiments");
        exit(EXIT_FAILURE);
    }
    command->threads_num = atoi(argv[1]);
    command->K = atoi(argv[2]);
    command->tour_now = atoi(argv[3]);
    command->first_player_score = atoi(argv[4]);
    command->second_player_score = atoi(argv[5]);
    command->experiments = atoi(argv[6]);
}


int main(int argc, char *argv[]) {
    Command command;
    parse_command_line(argc, argv, &command);

    command.threads_num = min(command.threads_num, command.experiments);

    pthread_t *pthread_ids = (pthread_t *)calloc(command.threads_num, sizeof(pthread_t));

    Params *params = (Params *)calloc(command.threads_num, sizeof(Params));
    init(params, &command);

    for (int i = 0; i < command.threads_num; ++i) {
        pthread_create(&pthread_ids[i], NULL, thread_function, (void *) &params[i]);
    }

    for (int j = 0; j < command.threads_num; ++j) {
        pthread_join(pthread_ids[j], NULL);
    }

    int first_won = 0;
    int second_won = 0;

    for (int i = 0; i < command.threads_num; ++i) {
        first_won += params[i].first_won;
        second_won += params[i].second_won;
    }
    printf("First chances: %d/%d = %LF%%\n", first_won, command.experiments, (long double)first_won / command.experiments * 100);
    printf("Second chances: %d/%d = %LF%%\n", second_won, command.experiments, (long double)second_won / command.experiments * 100);

    free(pthread_ids);
    free(params);

    return 0;
}