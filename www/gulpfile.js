let gulp = require('gulp'),
  sass = require('gulp-sass')(require('sass')),
  autoprefixer = require('gulp-autoprefixer'),
  rename = require('gulp-rename'),
  cleanCSS = require('gulp-clean-css'),
  gcmq = require('gulp-group-css-media-queries');

gulp.task('default', () => {
  return gulp.src(['node_modules/bootstrap/scss/bootstrap.scss', 'scss/*.scss'])
    .pipe(sass({
      outputStyle: 'compressed'
    }))
    .pipe(autoprefixer({
      overrideBrowserslist: ['last 2 years'],
      cascade: false
    }))
    .pipe(rename({ extname: '.min.css' }))
    .pipe(gcmq())
    .pipe(cleanCSS({compatibility: 'ie8'}))
    .pipe(gulp.dest('dist/css'))
    .pipe(rename({
      extname: '.min.css'
    }));
});
