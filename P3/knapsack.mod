# ILP model for knapsack

param n;
param costBound;

param value {0..n-1} >= 0;
param cost {0..n-1} >=0;

var x {0..n-1} binary;

param totalCost default 0;
param totalValue default 0;

# Objective function
maximize objective: sum{i in 0..n-1} value[i] * x[i];

# Constraint
subject to costConstraint: sum{i in 0..n-1} cost[i] * x[i] <= costBound;

data;
include data/knapsack8.dat;
solve;

print "Used items:";
printf "#\tCost\tValue\n";
for {i in 0..n-1} {
    if x[i] > 0 then {
        printf "%0s\t%d\t%d\n", i, cost[i], value[i];
        let totalCost := totalCost + cost[i];
        let totalValue := totalValue + value[i];
    }
}

printf "\n";
display costBound;
display totalCost;
display totalValue;
quit;

