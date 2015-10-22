# libtimefilter benchmarks #

<p>This page presents timing accuracy benchmarks produced using<br>
libtimefilter on various hardware.</p>

<p>You can easily test your own hardware, check the<br>
<a href='http://code.google.com/p/libtimefilter/source/browse/trunk/README'>README</a> for more.<p />

<p>Tested soundcards:</p>

<ul>
<li><a href='#hda'>Intel 82801I (ICH9 Family) HD Audio</a></li>
<li><a href='#firebox'>Presonus Firebox (Firewire)</a></li>
<li><a href='#rme'>RME DSP 9652</a></li>
<li><a href='#lexicon'>Lexicon Omega (USB)</a></li>
</ul>

<blockquote>

<hr />

</blockquote>

<h2><a>Intel 82801I (ICH9 Family) HD Audio</a></h2>

<p><b>./measure output:</b></p>
<pre>

=========================<br>
Measuring timing accuracy<br>
=========================<br>
Output: measures/data.mat<br>
<br>
Legend (all in ms):<br>
audiotime drift: the difference between audio and system time<br>
filter drift: the difference between filter and system time<br>
systime jitter: the average cycle duration error (+-), in system time<br>
filter jitter: the average cycle duration error (+-), in filter time<br>
<br>
DLL bandwidth: 1.50Hz<br>
Lines are printed every 5 second(s). Will run for 1 minute(s).<br>
+-----------------+---------------+----------------+----------------+<br>
| audiotime drift | filter drift  | systime jitter | filter jitter  |<br>
+-----------------+---------------+----------------+----------------+<br>
|        0.000000 |      0.000000 |       0.000000 |       0.000000 |<br>
|       -0.042677 |     -0.005960 |       0.007659 |       0.004112 |<br>
|       -0.057936 |      0.001907 |       0.008023 |       0.004266 |<br>
|       -0.109434 |     -0.020027 |       0.007816 |       0.004146 |<br>
|       -0.126600 |     -0.012636 |       0.008133 |       0.004247 |<br>
|       -0.150919 |     -0.003338 |       0.008178 |       0.004285 |<br>
|       -0.173807 |      0.002861 |       0.008090 |       0.004228 |<br>
|       -0.189781 |      0.015497 |       0.008185 |       0.004239 |<br>
|       -0.235796 |      0.002384 |       0.008585 |       0.004399 |<br>
|       -0.242710 |      0.015020 |       0.008568 |       0.004379 |<br>
|       -0.293732 |     -0.019550 |       0.008812 |       0.004506 |<br>
|       -0.321627 |     -0.015974 |       0.009471 |       0.004911 |<br>
|       -0.351667 |     -0.014305 |       0.010942 |       0.005825 |<br>
+-----------------+---------------+----------------+----------------+<br>
<br>
average jitter correction factor: x 1.88<br>
<br>
</pre>

<p><b>Long graph: </b></p>
<img src='http://www.samalyse.com/code/pendule/benchmarks/intel_hda_82801I/graph.png' border='1' />

<p><b>Short graph: </b></p>

<img src='http://www.samalyse.com/code/pendule/benchmarks/intel_hda_82801I/periodgraph.png' border='1' />

<blockquote>

<hr />

</blockquote>

<h2><a>Presonus Firebox (Firewire)</a></h2>

<p><b>./measure output:</b></p>
<pre>

=========================<br>
Measuring timing accuracy<br>
=========================<br>
Output: measures/data.mat<br>
<br>
Legend (all in ms):<br>
audiotime drift: the difference between audio and system time<br>
filter drift: the difference between filter and system time<br>
systime jitter: the average cycle duration error (+-), in system time<br>
filter jitter: the average cycle duration error (+-), in filter time<br>
<br>
DLL bandwidth: 1.50Hz<br>
Lines are printed every 5 second(s). Will run for 1 minute(s).<br>
+-----------------+---------------+----------------+----------------+<br>
| audiotime drift | filter drift  | systime jitter | filter jitter  |<br>
+-----------------+---------------+----------------+----------------+<br>
|        0.000000 |      0.000000 |       0.000000 |       0.000000 |<br>
|       -1.318455 |     -1.333952 |       1.323849 |       0.616647 |<br>
|       -1.282454 |     -1.171827 |       1.311712 |       0.614624 |<br>
|        0.551701 |      1.498461 |       1.307679 |       0.613512 |<br>
|        0.435591 |      1.498699 |       1.305910 |       0.613384 |<br>
|        0.316620 |      1.499414 |       1.304886 |       0.613348 |<br>
|       -0.720263 |     -0.337124 |       1.303541 |       0.613354 |<br>
|       -0.837326 |     -0.325680 |       1.303012 |       0.613275 |<br>
|       -0.962496 |     -0.338793 |       1.302602 |       0.613174 |<br>
|       -2.094269 |     -1.167536 |       1.301845 |       0.612773 |<br>
|       -2.215624 |     -1.182795 |       1.302857 |       0.613275 |<br>
|       -0.393629 |      1.540184 |       1.303022 |       0.613679 |<br>
|       -0.694275 |      1.479387 |       1.301451 |       0.613083 |<br>
+-----------------+---------------+----------------+----------------+<br>
<br>
average jitter correction factor: x 2.12<br>
<br>
</pre>

<p><b>Long graph: </b></p>

<img src='http://www.samalyse.com/code/pendule/benchmarks/firebox/graph.png' border='1' />

<p><b>Short graph: </b></p>
<img src='http://www.samalyse.com/code/pendule/benchmarks/firebox/periodgraph.png' border='1' />

<br>
<br>
<hr /><br>
<br>
<br>
<br>
<h2><a>RME DSP 9652</a></h2>

<p><b>./measure output:</b></p>
<pre>
not available<br>
</pre>

<p><b>Long graph: </b></p>
<img src='http://www.samalyse.com/code/pendule/benchmarks/rme9652_01/graph.png' border='1' />

<p><b>Short graph: </b></p>
<img src='http://www.samalyse.com/code/pendule/benchmarks/rme9652_01/periodgraph.png' border='1' />

<br>
<br>
<hr /><br>
<br>
<br>
<br>
<h2><a>Lexicon Omega (USB)</a></h2>

<p><b>./measure output:</b></p>
<pre>
not available<br>
<br>
</pre>

<p><b>Long graph: </b></p>
<img src='http://www.samalyse.com/code/pendule/benchmarks/audio_usb-lexicon_omega-01/graph.png' border='1' />

<p><b>Short graph: </b></p>
<img src='http://www.samalyse.com/code/pendule/benchmarks/audio_usb-lexicon_omega-01/periodgraph.png' border='1' />