"""
Beau Albritton
graph.py

This is a simple graphing script that is called by main.cpp.
Takes a few arguments:
 * total lines parsed
 * amount of correct predictions
 * bit predictor size (for labeling + output)
 * file name (for labeling + output)

"""

#imports
import sys, os
import matplotlib.pyplot as plt

#Getting parameters as system arguments
total = int(sys.argv[1])
correct = int(sys.argv[2])
bitCountAmt = int(sys.argv[3])
outFile = sys.argv[4]

#now incorrect is just the complement of correct
incorrect = total - correct

#Graph setup for labels & display
labels = ['Correct', 'Incorrect']
values = [correct, incorrect]
#Converting to percentages
percentages = [correct/total*100, incorrect/total*100]
#Nice colors that aren't so ugly
colors = ['#6495ed', '#f08080']

# Create pie chart because I don't like bar charts
plt.figure(figsize=(8, 6))
plt.pie(values, labels=labels, colors=colors, autopct='%1.1f%%', startangle=90)

#Using f string for title and file output. Unique for each
plt.title(f"Accuracy for {bitCountAmt}BitPredictor with {outFile} as input file ({correct}/{total})")
outFile = os.path.basename(outFile)
plt.savefig(f"{bitCountAmt}BitPredictor-{outFile}-Results.png")
plt.show()
