#include <bits/stdc++.h>
#include "lcgrand.h"

using namespace std;

// #include "lcgrand.c"

#define Q_LIMIT 100
#define BUSY 1
#define IDLE 0

ifstream infile;
fstream outfile;

int next_event_type, num_custs_delayed, num_delays_required, num_events, num_in_q, server_status;

double area_num_in_q, area_server_status, mean_interarrival, mean_service, sim_time, time_arrival[Q_LIMIT + 1], time_last_event, time_next_event[3], total_of_delays;

class SingleServerQueue
{

public:
    SingleServerQueue(double mean_i, double mean_s, int num_delayes_re)
    {
        num_events = 2;
        // number of events are defined here
        sim_time = 0.0;

        server_status = IDLE;
        num_in_q = 0;
        time_last_event = 0.0;

        num_custs_delayed = 0;
        total_of_delays = 0.0;
        area_num_in_q = 0.0;
        area_server_status = 0.0;

        time_next_event[1] = sim_time + expon(mean_interarrival);
        // expon was in book
        time_next_event[2] = 1.0e+30;

        mean_interarrival = mean_i;
        mean_service = mean_s;
        num_delays_required = num_delayes_re;
    }

    int get_num_custs_delayed()
    {
        return num_custs_delayed;
    }

    int get_num_delays_required()
    {
        return num_delays_required;
    }

    int get_next_event_type()
    {
        return next_event_type;
    }

    void arrive()
    {
        double delay;

        time_next_event[1] = sim_time + expon(mean_interarrival);
        // expon was in book

        if (server_status == BUSY)
        {
            num_in_q++;

            if (num_in_q > Q_LIMIT)
            {
                outfile << "Overflow of the array time_arrival at " << sim_time << endl;
                exit(2);
            }
            time_arrival[num_in_q] = sim_time;
        }
        else
        {
            delay = 0.0;
            total_of_delays = total_of_delays + delay;

            num_custs_delayed++;
            server_status = BUSY;

            time_next_event[2] = sim_time + expon(mean_service);
            // expon was in book
        }
    }

    void timing()
    {
        int i;
        double min_time_next_event = 1.0e+29;

        next_event_type = 0;

        for (i = 1; i <= num_events; i++)
        {
            if (time_next_event[i] < min_time_next_event)
            {
                min_time_next_event = time_next_event[i];
                next_event_type = i;
            }
        }

        if (next_event_type == 0)
        {
            outfile << "Event list empty at time " << sim_time << endl;
            exit(1);
        }

        sim_time = min_time_next_event;
        // cout << "sim_time: " << sim_time << endl;
    }

    void depart()
    {
        int i;
        double delay;

        if (num_in_q == 0)
        {
            server_status = IDLE;
            time_next_event[2] = 1.0e+30;
        }
        else
        {
            num_in_q--;

            delay = sim_time - time_arrival[1];
            total_of_delays = total_of_delays + delay;

            num_custs_delayed++;
            time_next_event[2] = sim_time + expon(mean_service);
            // expon was in book

            for (i = 1; i <= num_in_q; i++)
            {
                time_arrival[i] = time_arrival[i + 1];
            }
        }
    }

    void report()
    {
        outfile << "Average delay in queue " << total_of_delays / num_custs_delayed << " minutes" << endl;

        outfile << "Average number in queue " << area_num_in_q / sim_time << endl;

        outfile << "Server utilization " << area_server_status / sim_time << endl;

        outfile << "Time simulation ended " << sim_time << " minutes" << endl;
    }

    void update_time_avg_stats()
    {
        double time_since_last_event;

        time_since_last_event = sim_time - time_last_event;
        time_last_event = sim_time;

        area_num_in_q = area_num_in_q + (num_in_q * time_since_last_event);
        area_server_status = area_server_status + (server_status * time_since_last_event);
    }

    double expon(double mean)
    {
        // double u;

        // u = rand() * 1.0 / RAND_MAX;
        // return -mean * log(u);
        return -mean * log(lcgrand(1));
    }
};

int main()
{
    infile.open("mm1.in", ios::in);
    outfile.open("mm1.out", ios::out);

    infile >> mean_interarrival >> mean_service >> num_delays_required;

    cout << "Single-server queueing system" << endl
         << endl;
    cout << "Mean interarrival time " << mean_interarrival << " minutes" << endl
         << endl;
    cout << "Mean service time " << mean_service << " minutes" << endl
         << endl;
    cout << "Number of customers " << num_delays_required << endl
         << endl;

    // num_events = 2;

    // initialize();

    SingleServerQueue ssq(mean_interarrival, mean_service, num_delays_required);

    while (ssq.get_num_custs_delayed() < ssq.get_num_delays_required())
    {
        ssq.timing();
        ssq.update_time_avg_stats();

        if (ssq.get_next_event_type() == 1)
        {
            ssq.arrive();
        }
        else if (ssq.get_next_event_type() == 2)
        {
            ssq.depart();
        }
    }

    ssq.report();

    infile.close();
    outfile.close();

    return 0;
}
