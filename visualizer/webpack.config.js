const path = require('path');

module.exports = {
	mode: 'development',
	entry: './src/main.ts',
	devtool: 'inline-source-map',
	module: {
		rules: [
			{
				test: /\.tsx?$/,
				use: 'ts-loader',
				exclude: /node_modules/
			},
			{
				test: /\.scss$/,
				use: ['style-loader', 'css-loader', 'sass-loader'],
				exclude: /node_modules/
			}
		]
	},
	resolve: {
		extensions: ['.tsx', '.ts', '.js', '.sass']
	},
	output: {
		filename: 'bundle.js',
		publicPath: '/dist/',
		path: path.resolve(__dirname, 'dist')
	},
};
