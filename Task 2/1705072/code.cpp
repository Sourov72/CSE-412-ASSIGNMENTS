#include <bits/stdc++.h>
#include "lcgrand.h"

using namespace std;

int amount, bigs, initial_inv_level, inv_level, next_event_type, num_events,
    num_months, num_values_demand, smalls;

double area_holding, area_shortage, holding_cost, incremental_cost, maxlag,
    mean_interdemand, minlag, prob_distrib_demand[26], setup_cost,
    shortage_cost, sim_time, time_last_event, time_next_event[5],
    total_ordering_cost;

ifstream infile;
fstream outfile;

double uniform(double a, double b)
{
    return a + lcgrand(1) * (b - a);
}

double expon(double mean) /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log(lcgrand(1));
}

int random_integer(double prob_distrib[])
{
    int i;
    double u;

    u = lcgrand(1);

    for (i = 1; i <= num_values_demand; ++i)
    {
        if (u < prob_distrib[i])
        {
            return i;
        }
    }
    return i;

    // for (i = 1; u >= prob_distrib[i]; i++)
    //     ;

    // return i;
}



void initialize(void)
{
    sim_time = 0.0;

    inv_level = initial_inv_level;

    time_last_event = 0.0;

    // num_events = 4;

    time_next_event[1] = 1.0e+30;
    time_next_event[2] = sim_time + expon(mean_interdemand);
    // in co-pilot 1 & 2 were exchanged
    time_next_event[3] = num_months;
    time_next_event[4] = 0.0;

    area_holding = 0.0;
    area_shortage = 0.0;
    total_ordering_cost = 0.0;
}

void timing(void) /* Timing function. */
{
    int i;
    double min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event)
        {
            min_time_next_event = time_next_event[i];
            next_event_type = i;
        }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0)
    {

        /* The event list is empty, so stop the simulation */

        outfile << "Event list empty at time " << sim_time << endl;
        exit(1);
    }

    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;
}

void order_arrival(void)
{
    inv_level += amount;

    time_next_event[1] = 1.0e+30;
}

void demand(void)
{
    inv_level -= random_integer(prob_distrib_demand);

    time_next_event[2] = sim_time + expon(mean_interdemand);
}

void evaluate(void)
{
    if (inv_level < smalls)
    {
        amount = bigs - inv_level;
        total_ordering_cost += setup_cost + incremental_cost * amount;
        time_next_event[1] = sim_time + uniform(minlag, maxlag);
    }

    time_next_event[4] = sim_time + 1.0;
}

void report(void)
{
    double avg_holding_cost, avg_ordering_cost, avg_shortage_cost;

    avg_ordering_cost = total_ordering_cost / num_months;
    avg_holding_cost = holding_cost * area_holding / num_months;
    avg_shortage_cost = shortage_cost * area_shortage / num_months;

    outfile << endl
         << endl;

    outfile << setw(3) << smalls << " " << setw(3) << bigs << "    " << setw(12) << avg_ordering_cost + avg_holding_cost + avg_shortage_cost << "          " << setw(12) << avg_ordering_cost << "          " << setw(12) << avg_holding_cost << "          " << setw(12) << avg_shortage_cost << endl;
}

void update_time_avg_stats(void)
{
    double time_since_last_event;

    time_since_last_event = sim_time - time_last_event;
    time_last_event = sim_time;

    if (inv_level < 0)
    {
        area_shortage -= inv_level * time_since_last_event;
    }
    else
    {
        area_holding += inv_level * time_since_last_event;
    }
}





int main()
{
    int i, num_policies;

    infile.open("mm1.in", ios::in);
    outfile.open("mm1.out", ios::out);

    num_events = 4;

    infile >> initial_inv_level >> num_months >> num_policies >> num_values_demand >> mean_interdemand >> setup_cost >> incremental_cost >> holding_cost >> shortage_cost >> minlag >> maxlag;

    for (i = 1; i <= num_values_demand; ++i)
    {
        infile >> prob_distrib_demand[i];
    }

    outfile << "Single-product inventory system" << endl
         << endl;
    outfile << "Initial inventory level " << initial_inv_level << endl
         << endl;
    outfile << "Number of demand sizes " << num_values_demand << endl
         << endl;

    outfile << "Distribution function of demand sizes  ";
    for (i = 1; i <= num_values_demand; ++i)
    {
        outfile << prob_distrib_demand[i] << " ";
    }

    outfile << endl
         << endl;
    outfile << "Mean interdemand time " << mean_interdemand << endl
         << endl;

    outfile << "Delivery lag range " << minlag << " to " << maxlag << " months" << endl
         << endl;

    outfile << "Length of simulation " << num_months << " months" << endl
         << endl;

    outfile << "K = " << setup_cost << " i = " << incremental_cost << " h = " << holding_cost << " pi = " << shortage_cost << endl
         << endl;

    outfile << "Number of policies " << num_policies << endl
         << endl;

    outfile << "                Average               Average               Average               Average" << endl;

    outfile << " Policy       total cost           ordering cost";

    outfile << "         holding cost          shortage cost" << endl;

    for (i = 1; i <= num_policies; i++)
    {
        infile >> smalls >> bigs;
        initialize();

        do
        {
            timing();
            update_time_avg_stats();

            if (next_event_type == 1)
            {
                order_arrival();
            }
            else if (next_event_type == 2)
            {
                demand();
            }
            else if (next_event_type == 3)
            {
                report();
            }
            else if (next_event_type == 4)
            {
                evaluate();
            }
        } while (next_event_type != 3);
    }

    infile.close();
    outfile.close();
    return 0;
}
