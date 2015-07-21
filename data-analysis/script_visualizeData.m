#------------------------------------------------------------------------------
# This is a script for visualizing LyraMark benchmarking tool results
# for different parameters of Lyra2RE algorithm.
#------------------------------------------------------------------------------
SAVE_IMAGES = false; # Set this true to save images to (eps and/or png) files.

# Set parameter that will be used for data extraction.
MAX_TIMECOST = 5;
MIN_COLS = 4;
MAX_COLS = 64;
MIN_ROWS = 4;
MAX_ROWS = 64;

data = csvread('benchmark.csv');
#data = csvread('benchmark_xeon_e3-1230v2.csv');
#data = csvread('../bin/Release/benchmark.csv');

rowCount = MAX_ROWS - MIN_ROWS + 1;
colCount = MAX_COLS - MIN_COLS + 1;


# Take data for the selected time costs
for timeCost = 1:MAX_TIMECOST

  visualData = data(data(:, 1) == timeCost, :);


  # Extract rows, cols and hashrates
  rows = zeros(rowCount, colCount);
  rows(:) = visualData(:, 2);

  cols = zeros(rowCount, colCount);
  cols(:) = visualData(:, 3);

  hashrates = zeros(rowCount, colCount);
  hashrates(:) = visualData(:, 4);

  # Create a 3-D plot
  hFigure = figure(timeCost);
  clf(hFigure);
  
  surf(cols, rows, hashrates);
  graphTitle = ['Lyra2RE on CPU: time cost = ' num2str(timeCost)];
  title(graphTitle);
  xlabel('Rows');
  ylabel('Columns');
  zlabel('kh/s');
  zlim([0.0 1000.0]);

  # Set viewpoint: azimuth = 135°, elevation = 45°
  view(135, 45);

  # Save graphs
  if SAVE_IMAGES
  
      #filenameEPS = ['lyra2re_cpu_timecost_' num2str(timeCost) '.eps'];
      #print(hFigure, filenameEPS, '-deps', '-color');
      filenamePNG = ['lyra2re_cpu_timecost_' num2str(timeCost) '.png'];
      saveas(hFigure, filenamePNG, 'png');
      
  endif
  
end