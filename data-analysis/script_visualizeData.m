#------------------------------------------------------------------------------
# This is a script for visualizing LyraMark benchmarking tool results
# for different parameters of Lyra2RE algorithm.
#------------------------------------------------------------------------------
SAVE_IMAGES = false; # Set this true to save images to (eps and/or png) files.

data = csvread('benchmark.csv');
#data = csvread('../bin/Release/benchmark.csv');

# Take data for the selected time costs
for timeCost = 1:10

  visualData = data(data(:, 1) == timeCost, :);


  # Extract rows, cols and hashrates
  rows = zeros(61,61);
  rows(:) = visualData(:, 2);

  cols = zeros(61,61);
  cols(:) = visualData(:, 3);

  hashrates = zeros(61,61);
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
  zlim([0.0 250.0]);

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